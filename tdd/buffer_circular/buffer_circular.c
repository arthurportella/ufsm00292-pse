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

int bc_cheio(const buffer_circular_t *b)
{
    return (b->contador == b->tamanho);
}

size_t bc_capacidade(const buffer_circular_t *b)
{
    return b->tamanho;
}

size_t bc_ocupacao(const buffer_circular_t *b)
{
    return b->contador;
}

int bc_le(buffer_circular_t *b, uint8_t *dado)
{
    if (bc_vazio(b))
    {
        return BC_ERRO_VAZIO;
    }

    *dado = b->dados[b->inicio];
    b->inicio = (b->inicio + 1) % b->tamanho;
    b->contador--;

    return BC_OK;
}

int bc_escreve(buffer_circular_t *b, uint8_t dado)
{
    size_t fim = (b->inicio + b->contador) % b->tamanho;

    b->dados[fim] = dado;
    b->contador++;

    return BC_OK;
}
