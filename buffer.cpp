#include <cstdio>
#include <cstdint>


class BufferBits {
protected:
  FILE *arquivo;  
  uint8_t byte;   
  uint8_t n;     

public:
  BufferBits(FILE *arquivo);  
  uint8_t ocupados();         
  uint8_t livres();           
};

// Buffer de leitura de bits
class BufferBitsLeitura : public BufferBits {
public:
  BufferBitsLeitura(FILE *arquivo); 
  uint8_t le_bit();                 
};

// Buffer de escrita de bits
class BufferBitsEscrita : public BufferBits {
public:
  BufferBitsEscrita(FILE *arquivo);  
  void escreve_bit(uint8_t bit);    
  void descarrega();                
};


int DEBUG_BITS = 1;
int VALORES[8]={128,64,32,16,8,4,2,1};


int main(int argc, char *argv[])
{
  FILE *f = fopen(argv[1], "rb");



  fclose(f);
  return 0;
}


void escrever_binario(uint8_t numero)
{
  for (int i = 128; i > 0; i >>= 1)
    printf("%c", numero & i ? '1' : '0');
}

BufferBits::BufferBits(FILE *arquivo) :
  arquivo(arquivo),
  byte(0),
  n(0)
{ }

uint8_t BufferBits::ocupados()
{
  return n;
}

uint8_t BufferBits::livres()
{
  return 8 - n;
}

BufferBitsLeitura::BufferBitsLeitura(FILE *f) :
  BufferBits(f)
{ }

uint8_t BufferBitsLeitura::le_bit()
{

  void* aux=&byte;
  if (n == 0){
    int test=fread(aux, 1, 1, arquivo);
    if(test!=1){
    return 2;
    }
    n = 8;
}




  if (DEBUG_BITS) printf("n: %d, byte: %d (", n, byte);
  if (DEBUG_BITS) escrever_binario(byte);

  
  uint8_t bit;
  bit = (VALORES[n-1] & byte) ? 1 : 0;
  n-=1;

  if (DEBUG_BITS) printf(") --> %d(", byte);
  if (DEBUG_BITS) escrever_binario(byte);
  if (DEBUG_BITS) printf("), bit: %d\n", bit);

  return bit;
}

BufferBitsEscrita::BufferBitsEscrita(FILE *f) :
  BufferBits(f)
{ }

void BufferBitsEscrita::escreve_bit(uint8_t bit)
{
  if (DEBUG_BITS) printf("bit: %d, n: %d, byte: %d (", bit, n, byte);
  if (DEBUG_BITS) escrever_binario(byte);

  
  byte = byte | (VALORES[n-1]*bit);
  n+=1;

  if (DEBUG_BITS) printf(") --> %d(", byte);
  if (DEBUG_BITS) escrever_binario(byte);
  if (DEBUG_BITS) printf(")\n");

  if (n == 8)
    descarrega();
}

void BufferBitsEscrita::descarrega()
{
    if(byte>=1){
        void *aux=&byte;
        fwrite(aux,1,1,arquivo);
        byte=0;
        n=0;
        }

}
