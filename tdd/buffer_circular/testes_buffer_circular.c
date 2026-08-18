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
/* Ciclo 3 - escrita, leitura do dado e ordem FIFO                     */
/* ------------------------------------------------------------------ */

static char *teste_escreve_retorna_ok(void)
{
    buffer_circular_t b;
    uint8_t area[8];

    bc_inicializa(&b, area, sizeof(area));
    verifica("erro: bc_escreve() deveria retornar BC_OK",
             bc_escreve(&b, 0x55) == BC_OK);
    return 0;
}

static char *teste_apos_escrever_buffer_nao_esta_vazio(void)
{
    buffer_circular_t b;
    uint8_t area[8];

    bc_inicializa(&b, area, sizeof(area));
    bc_escreve(&b, 0x55);
    verifica("erro: apos escrever, o buffer nao deveria estar vazio",
             bc_vazio(&b) == 0);
    verifica("erro: apos escrever um dado, a ocupacao deveria ser 1",
             bc_ocupacao(&b) == 1);
    return 0;
}

static char *teste_le_o_dado_escrito(void)
{
    buffer_circular_t b;
    uint8_t area[8];
    uint8_t dado = 0;

    bc_inicializa(&b, area, sizeof(area));
    bc_escreve(&b, 0x55);
    verifica("erro: bc_le() deveria retornar BC_OK",
             bc_le(&b, &dado) == BC_OK);
    verifica("erro: bc_le() deveria devolver o dado escrito (0x55)",
             dado == 0x55);
    return 0;
}

static char *teste_ordem_fifo(void)
{
    buffer_circular_t b;
    uint8_t area[8];
    uint8_t dado = 0;

    bc_inicializa(&b, area, sizeof(area));
    bc_escreve(&b, 1);
    bc_escreve(&b, 2);
    bc_escreve(&b, 3);

    bc_le(&b, &dado);
    verifica("erro: o primeiro dado lido deveria ser 1", dado == 1);
    bc_le(&b, &dado);
    verifica("erro: o segundo dado lido deveria ser 2", dado == 2);
    bc_le(&b, &dado);
    verifica("erro: o terceiro dado lido deveria ser 3", dado == 3);
    return 0;
}

static char *teste_apos_ler_tudo_buffer_fica_vazio(void)
{
    buffer_circular_t b;
    uint8_t area[8];
    uint8_t dado = 0;

    bc_inicializa(&b, area, sizeof(area));
    bc_escreve(&b, 1);
    bc_escreve(&b, 2);
    bc_le(&b, &dado);
    bc_le(&b, &dado);

    verifica("erro: apos ler todos os dados o buffer deveria estar vazio",
             bc_vazio(&b) == 1);
    verifica("erro: apos ler todos os dados a ocupacao deveria ser 0",
             bc_ocupacao(&b) == 0);
    return 0;
}

/* ------------------------------------------------------------------ */
/* Ciclo 4 - buffer cheio e protecao contra estouro                    */
/* ------------------------------------------------------------------ */

static char *teste_buffer_fica_cheio_na_capacidade(void)
{
    buffer_circular_t b;
    uint8_t area[4];
    int i;

    bc_inicializa(&b, area, sizeof(area));
    for (i = 0; i < 4; i++)
    {
        bc_escreve(&b, (uint8_t)i);
    }

    verifica("erro: apos escrever 4 dados em area de 4 bytes o buffer deveria estar cheio",
             bc_cheio(&b) == 1);
    verifica("erro: buffer cheio deveria ter ocupacao igual a capacidade",
             bc_ocupacao(&b) == bc_capacidade(&b));
    return 0;
}

static char *teste_escreve_em_buffer_cheio_retorna_erro(void)
{
    buffer_circular_t b;
    uint8_t area[4];
    int i;

    bc_inicializa(&b, area, sizeof(area));
    for (i = 0; i < 4; i++)
    {
        bc_escreve(&b, (uint8_t)i);
    }

    verifica("erro: escrever em buffer cheio deveria retornar BC_ERRO_CHEIO",
             bc_escreve(&b, 0xFF) == BC_ERRO_CHEIO);
    verifica("erro: escrita rejeitada nao deveria alterar a ocupacao",
             bc_ocupacao(&b) == 4);
    return 0;
}

static char *teste_buffer_cheio_nao_sobrescreve_dados(void)
{
    buffer_circular_t b;
    uint8_t area[4];
    uint8_t dado = 0;
    int i;

    bc_inicializa(&b, area, sizeof(area));
    for (i = 0; i < 4; i++)
    {
        bc_escreve(&b, (uint8_t)(i + 1));
    }
    bc_escreve(&b, 0xFF);   /* rejeitada */

    for (i = 0; i < 4; i++)
    {
        bc_le(&b, &dado);
        verifica("erro: a escrita rejeitada corrompeu os dados armazenados",
                 dado == (uint8_t)(i + 1));
    }
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

    /* ciclo 3 */
    executa_teste(teste_escreve_retorna_ok);
    executa_teste(teste_apos_escrever_buffer_nao_esta_vazio);
    executa_teste(teste_le_o_dado_escrito);
    executa_teste(teste_ordem_fifo);
    executa_teste(teste_apos_ler_tudo_buffer_fica_vazio);

    /* ciclo 4 */
    executa_teste(teste_buffer_fica_cheio_na_capacidade);
    executa_teste(teste_escreve_em_buffer_cheio_retorna_erro);
    executa_teste(teste_buffer_cheio_nao_sobrescreve_dados);

    return 0;
}
