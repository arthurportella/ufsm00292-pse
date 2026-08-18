/*
 * buffer_circular.h
 *
 * Modulo de manipulacao de um buffer circular (fila FIFO) com area de
 * memoria fornecida pelo usuario (sem alocacao dinamica).
 *
 * UFSM00292 - Projeto de Sistemas Embarcados
 */

#ifndef BUFFER_CIRCULAR_H
#define BUFFER_CIRCULAR_H

#include <stddef.h>
#include <stdint.h>

/* codigos de retorno */
#define BC_OK               0
#define BC_ERRO_PARAMETRO  -1
#define BC_ERRO_VAZIO      -3

typedef struct
{
    uint8_t *dados;     /* area de memoria fornecida pelo usuario */
    size_t   tamanho;   /* capacidade, em bytes                   */
    size_t   inicio;    /* indice de leitura (START)              */
    size_t   contador;  /* quantidade de dados armazenados        */
} buffer_circular_t;

int bc_inicializa(buffer_circular_t *b, uint8_t *area, size_t tamanho);
int bc_vazio(const buffer_circular_t *b);
int bc_cheio(const buffer_circular_t *b);
size_t bc_capacidade(const buffer_circular_t *b);
size_t bc_ocupacao(const buffer_circular_t *b);
int bc_le(buffer_circular_t *b, uint8_t *dado);

#endif /* BUFFER_CIRCULAR_H */
