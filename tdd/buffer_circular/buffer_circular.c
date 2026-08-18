/*
 * buffer_circular.c
 *
 * UFSM00292 - Projeto de Sistemas Embarcados
 */

#include "buffer_circular.h"

int bc_inicializa(buffer_circular_t *b, uint8_t *area, size_t tamanho)
{
    b->dados = area;
    b->tamanho = tamanho;
    b->inicio = 0;
    b->contador = 0;

    return BC_OK;
}

int bc_vazio(const buffer_circular_t *b)
{
    return (b->contador == 0);
}
