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
/* Ciclo 2 - capacidade, ocupacao e leitura de buffer vazio            */
/* ------------------------------------------------------------------ */

static char *teste_buffer_novo_nao_esta_cheio(void)
{
    buffer_circular_t b;
    uint8_t area[8];

    bc_inicializa(&b, area, sizeof(area));
    verifica("erro: buffer recem-inicializado nao deveria estar cheio",
             bc_cheio(&b) == 0);
    return 0;
}

static char *teste_capacidade_eh_o_tamanho_informado(void)
{
    buffer_circular_t b;
    uint8_t area[8];

    bc_inicializa(&b, area, sizeof(area));
    verifica("erro: bc_capacidade() deveria retornar 8",
             bc_capacidade(&b) == 8);
    return 0;
}

static char *teste_ocupacao_inicial_eh_zero(void)
{
    buffer_circular_t b;
    uint8_t area[8];

    bc_inicializa(&b, area, sizeof(area));
    verifica("erro: bc_ocupacao() de buffer novo deveria retornar 0",
             bc_ocupacao(&b) == 0);
    return 0;
}

static char *teste_le_buffer_vazio_retorna_erro(void)
{
    buffer_circular_t b;
    uint8_t area[8];
    uint8_t dado = 0xAA;

    bc_inicializa(&b, area, sizeof(area));
    verifica("erro: ler de buffer vazio deveria retornar BC_ERRO_VAZIO",
             bc_le(&b, &dado) == BC_ERRO_VAZIO);
    verifica("erro: ler de buffer vazio nao deveria alterar o destino",
             dado == 0xAA);
    return 0;
}

/* ------------------------------------------------------------------ */

static char *executa_testes(void)
{
    /* ciclo 1 */
    executa_teste(teste_inicializa_retorna_ok);
    executa_teste(teste_buffer_novo_esta_vazio);

    /* ciclo 2 */
    executa_teste(teste_buffer_novo_nao_esta_cheio);
    executa_teste(teste_capacidade_eh_o_tamanho_informado);
    executa_teste(teste_ocupacao_inicial_eh_zero);
    executa_teste(teste_le_buffer_vazio_retorna_erro);

    return 0;
}
