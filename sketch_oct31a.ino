#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/eeprom.h> 
#include <util/delay.h>

// ===== PINES ===== //
#define PIN_RESET_PULSE PB1     // Pin 6: Pulso de Reset Final (AHORA: LOW es Inactivo, HIGH es Activo)
#define PIN_MODO_PRUEBAS PB4    // Pin 3: Detección de Modo Pruebas (LOW=Pruebas)
#define PIN_MULTIPLIER PB3      // Multiplicador: Pin 4 (Puente a GND para x2)
#define PIN_MODO_INDICADOR PB0  // Pin 5: Indicador LED (Modo/Multiplicador)
#define PIN_SIGNAL_PULSE PB2    // Pin 7: Señalización Tiempo Restante

// ===== CONFIGURACIÓN TEMPORAL ===== //
#define WDT_INTERVAL_8S         
#define RESET_PULSE_MS 200      
#define SIGNAL_PULSE_MS 200     // Duración de cada pulso de señalización (200ms)

// --- CONSTANTES DE TIEMPO ---
#define CYCLES_PER_HOUR 450     
#define CYCLES_NORMAL 5400      
#define CYCLES_PRUEBAS 8        // 🔥 MODIFICADO: 60 segundos / 8 segundos = 7.5 -> 8 ciclos (1 minuto)
#define NUMERATOR 12            

#define EEPROM_SIGNAL_INTERVAL 1 // CADA X CICLOS SE MUESTRA UN NUMERO DE PULSO EN EL LED SIGNAL QUE DETERMINA CUANTO FALTA PARA EL RESET

#define INTERVAL_NORMAL (CYCLES_NORMAL / 12) 

// --- EEPROM Y CONTEO ---
#define EEPROM_ADDR_WDT_CNT 0x00 
#define EEPROM_WRITE_INTERVAL (CYCLES_PER_HOUR * 2)


// ⚠️ Variables Globales
volatile uint16_t wdt_cnt = 0; 
uint16_t total_cycles_target = 0; 
uint16_t intervalo_pulso; 

// -------------------------------------------------------------
// Watchdog Interrupt Service Routine (Se ejecuta cada 8 segundos)
// -------------------------------------------------------------
ISR(WDT_vect) {
    wdt_cnt++;    

        // --- LÓGICA DE SEÑALIZACIÓN POR DOCEAVA PARTE EN CADA CICLO (PB2) ---
    if (wdt_cnt % EEPROM_SIGNAL_INTERVAL == 0  & !(PINB & _BV(PIN_MODO_PRUEBAS)) ) {
        // Nota: He comentado la condición para que la señalización se muestre en AMBOS modos (Pruebas/Normal)
        // Si desea que SOLO se muestre en Modo Pruebas, descomente la condición anterior.
        
        uint16_t ciclos_restantes = total_cycles_target - wdt_cnt;
        uint8_t partes_restantes;
        
        partes_restantes = (ciclos_restantes + intervalo_pulso - 1) / intervalo_pulso;
        
        for (uint8_t i = 0; i < partes_restantes; i++) {
            PORTB &= ~_BV(PIN_SIGNAL_PULSE);  
            _delay_ms(SIGNAL_PULSE_MS);
            PORTB |= _BV(PIN_SIGNAL_PULSE);   
            _delay_ms(SIGNAL_PULSE_MS); 
        }

        PORTB &= ~_BV(PIN_SIGNAL_PULSE); 
    }
    
    // --- LÓGICA DEL INDICADOR DE MODO (PB0) - MODO PARPADEO x2 ---
    // Si el multiplicador está activo (PB3 LOW), el indicador parpadea
    if (!(PINB & _BV(PIN_MULTIPLIER))) { // Lectura directa del pin PB3
        // Parpadeo cada 8s (cambia de estado en cada interrupción)
        if (wdt_cnt % 2 == 0) {
            PORTB |= _BV(PIN_MODO_INDICADOR); 
        } else {
            PORTB &= ~_BV(PIN_MODO_INDICADOR); 
        }
    }
    
    // --- Lógica de EEPROM ---
    if (wdt_cnt % EEPROM_WRITE_INTERVAL == 0) {
        eeprom_update_word((uint16_t*)EEPROM_ADDR_WDT_CNT, wdt_cnt);
    }
}

// -------------------------------------------------------------
// Funciones de Configuración
// -------------------------------------------------------------

void setup_watchdog() {
    MCUSR &= ~_BV(WDRF);
    WDTCR |= _BV(WDCE) | _BV(WDE); 
    WDTCR = _BV(WDTIE) | _BV(WDP3) | _BV(WDP0);
}

void enter_sleep() {
    ACSR |= _BV(ACD);     
    ADCSRA &= ~_BV(ADEN); 
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    sei();
    sleep_cpu();
    sleep_disable();
}

void setup() {
    // --- 1. CONFIGURACIÓN INICIAL DE PINES ---
    // PB0, PB1, PB2 como Salidas
    DDRB |= _BV(PIN_RESET_PULSE) | _BV(PIN_SIGNAL_PULSE) | _BV(PIN_MODO_INDICADOR); 
    
    // 🔥 INVERTIDO: Reset (PB1) a LOW (Inactivo), Estado Normal
    PORTB |= _BV(PIN_RESET_PULSE);
    PORTB &= ~_BV(PIN_SIGNAL_PULSE); 
    PORTB &= ~_BV(PIN_MODO_INDICADOR); 
    
    // PB4 (Modo Pruebas) y PB3 (Multiplicador) como entradas con PULL-UP
    DDRB &= ~_BV(PIN_MODO_PRUEBAS); // PB4
    PORTB |= _BV(PIN_MODO_PRUEBAS); // PB4 pull-up
    DDRB &= ~_BV(PIN_MULTIPLIER);    // PB3
    PORTB |= _BV(PIN_MULTIPLIER);    // PB3 pull-up

    // --- 2. DETERMINACIÓN DE MODO BASE Y CÁLCULO DE INTERVALO ---
    if (!(PINB & _BV(PIN_MODO_PRUEBAS))) { 
        // MODO BASE: PRUEBAS (PB4 LOW)
        total_cycles_target = CYCLES_PRUEBAS; // 8 ciclos
        
        // Cálculo de intervalo pulso para 1 minuto: round(8 / 12) = 1
        intervalo_pulso = (CYCLES_PRUEBAS + (NUMERATOR / 2)) / NUMERATOR; 
        
        PORTB |= _BV(PIN_MODO_INDICADOR); // Indicador ON
    } else {
        // MODO BASE: NORMAL (PB4 HIGH)
        total_cycles_target = CYCLES_NORMAL; 
        intervalo_pulso = INTERVAL_NORMAL;
        
        PORTB &= ~_BV(PIN_MODO_INDICADOR); // Indicador OFF
    }

    // --- 3. APLICAR MULTIPLICADOR x2 (LECTURA DIRECTA) ---
    if (!(PINB & _BV(PIN_MULTIPLIER))) { // PB3 LOW (Puente a GND)
        total_cycles_target *= 2; 
        intervalo_pulso *= 2;
    }

    // --- 4. LECTURA Y AJUSTE DE CONTADOR (EEPROM) ---
    wdt_cnt = eeprom_read_word((uint16_t*)EEPROM_ADDR_WDT_CNT);
    
    if (wdt_cnt >= total_cycles_target) {
         wdt_cnt = 0;
         eeprom_update_word((uint16_t*)EEPROM_ADDR_WDT_CNT, wdt_cnt);
    }

    setup_watchdog();
}

void loop() {
    enter_sleep(); 
    _delay_ms(50); 

    if (wdt_cnt >= total_cycles_target) {
        wdt_cnt = 0; 
        eeprom_update_word((uint16_t*)EEPROM_ADDR_WDT_CNT, wdt_cnt);

        PORTB &= ~_BV(PIN_RESET_PULSE); 
        _delay_ms(RESET_PULSE_MS);
        PORTB |= _BV(PIN_RESET_PULSE); 

    }
}