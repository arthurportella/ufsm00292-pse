/*
 * testes_buffer_circular.c
 *
 * Testes unitarios do modulo de buffer circular, escritos segundo a
 * metodologia TDD (test driven development).
 *
 * UFSM00292 - Projeto de Sistemas Embarcados
 */

#include <stdio.h>
#include <stdlib.h>

#include "buffer_circular.h"

/* macros de testes */
#define verifica(mensagem, teste) do { if (!(teste)) return mensagem; } while (0)
#define executa_teste(teste) do { char *mensagem = teste(); testes_executados++; \
                                if (mensagem) return mensagem; } while (0)

int testes_executados = 0;

static char *executa_testes(void);

int main(void)
{
    char *resultado = executa_testes();

    if (resultado != 0)
    {
        printf("%s\n", resultado);
    }
    else
    {
        printf("TODOS OS TESTES PASSARAM\n");
    }
    printf("Testes executados: %d\n", testes_executados);

    return resultado != 0;
}

/* ------------------------------------------------------------------ */
/* Ciclo 1 - criacao do buffer                                         */
/* ------------------------------------------------------------------ */

static char *teste_inicializa_retorna_ok(void)
{
    buffer_circular_t b;
    uint8_t area[8];

    verifica("erro: bc_inicializa() deveria retornar BC_OK",
             bc_inicializa(&b, area, sizeof(area)) == BC_OK);
    return 0;
}

static char *teste_buffer_novo_esta_vazio(void)
{
    buffer_circular_t b;
    uint8_t area[8];

    bc_inicializa(&b, area, sizeof(area));
    verifica("erro: buffer recem-inicializado deveria estar vazio",
             bc_vazio(&b) == 1);
    return 0;
}

/* ------------------------------------------------------------------ */

static char *executa_testes(void)
{
    /* ciclo 1 */
    executa_teste(teste_inicializa_retorna_ok);
    executa_teste(teste_buffer_novo_esta_vazio);

    return 0;
}
