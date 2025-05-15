
}

// Función para rotar bits (izquierda)
void rotar_izquierda(unsigned char *entrada, int len, int shifts){
    for(int i = 0; i < shifts; i++){
        int first_bit = (entrada[0] & 0x80) >> 7;
        for(int j = 0; j < len-1; j++){
            entrada[j] = (entrada[j] << 1) | (entrada[j+1] >> 7);
        }
        entrada[len-1] = (entrada[len-1] << 1) | first_bit;
    }
}

// Generación de subclaves
// Esta función genera correctamente las 16 subclaves
void generar_subclaves(unsigned char *clave, unsigned char subclaves[16][6]) {
    unsigned char clave_permutada[7] = {0};
    permutar(PC1, 56, clave, clave_permutada);

    for(int i = 0; i < 16; i++){
        int shift = shifts[i];

        // Rotación izquierda para cada mitad (C y D)
        unsigned int C = (clave_permutada[0] << 20) | (clave_permutada[1] << 12) | (clave_permutada[2] << 4) | (clave_permutada[3] >> 4);
        unsigned int D = ((clave_permutada[3] & 0x0F) << 24) | (clave_permutada[4] << 16) | (clave_permutada[5] << 8) | clave_permutada[6];

        C = ((C << shift) | (C >> (28 - shift))) & 0x0FFFFFFF;
        D = ((D << shift) | (D >> (28 - shift))) & 0x0FFFFFFF;

        clave_permutada[0] = (C >> 20) & 0xFF;
        clave_permutada[1] = (C >> 12) & 0xFF;
        clave_permutada[2] = (C >> 4) & 0xFF;
        clave_permutada[3] = ((C & 0x0F) << 4) | ((D >> 24) & 0x0F);
        clave_permutada[4] = (D >> 16) & 0xFF;
        clave_permutada[5] = (D >> 8) & 0xFF;
        clave_permutada[6] = D & 0xFF;

        permutar(PC2, 48, clave_permutada, subclaves[i]);
    }
}

// Función Feistel corregida y completa para cada ronda
void ronda_des(unsigned char *L, unsigned char *R, unsigned char *subclave){
    unsigned char expandido[6] = {0}, resultado[4] = {0};

    // Expansión E de R