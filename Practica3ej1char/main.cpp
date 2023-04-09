#include <iostream>
#include <fstream>
#include <sstream>
#include <bitset>

const int numBits = 8;

using namespace std;

void invertBits(char *cdn, int max, int salto){             //invierte los bits(cadena de bits, tamaño grupos, cada cuantos bits es invierte)
    if(salto==1){                                  //si el parametro salto==1 se invierten todos los bits del grupo
        for(int i=0;i<=max-1;i++){
            if(*(cdn+i)=='0'){
                *(cdn+i)='1';
            }
            else if(*(cdn+i)=='1'){
                *(cdn+i)='0';
            }
        }
    }
    else{                                         //si el parametro salto!=1 se invierten tomando en cuenta el valor de salto
        for(int i=salto-1;i<=max-1;i+=salto){
            if(*(cdn+i)=='0'){
                *(cdn+i)='1';
            }
            else if(*(cdn+i)=='1'){
                *(cdn+i)='0';
            }
        }
    }
}

int contCerosUnos(char *cdn, int max){                       //cuenta la cantidad de '0' y '1' en el grupo indicado (cadena de bits,tamaño grupos)
    int ceros = 0;
    int unos = 0;
    int cont = 0;
    for(int i=0;i<=max-1;i++){
        if(*(cdn+i)=='0'){
            ceros++;
        }
        else{
            unos++;
        }
    }
    if(ceros==unos){
        cont = 0;
    }
    else if(ceros>unos){
        cont = 1;
    }
    else{
        cont = 2;
    }
    return cont;
}

void desBits(char *cdn, int max){             //desplaza un bit a la derecha en un grupo de bits (cadena de bits,tamaño grupos)
    char p = *(cdn+max-1);
    if(max==2){                              //si el grupo es de dos bits se intercambian
        if(*(cdn)=='0'){
            *(cdn+1)='0';
        }
        else if(*(cdn)=='1'){
            *(cdn+1)='1';
        }
    }
    else{
        for(int i=max-1;i>=1;i--){
            if(*(cdn+i-1)=='0'){
                *(cdn+i)='0';
            }
            else if(*(cdn+i-1)=='1'){
                *(cdn+i)='1';
            }
        }
    }
    *cdn = p;
}

int main()
{
    ifstream fin;                    //stream de entrada, lectura
    ofstream fout;                   //stream de salida, escritura
    char datos[1000];
    char aFuente[50];                //nombre archivo fuente
    char aSalida[50];                //nombre archivo de salida
    int seed;
    int metodo;
    int i=0;

    cout<<"Nombre del archivo fuente"<<endl;
    cin.getline(aFuente,50,'\n');

    cout<<"Nombre del archivo de salida"<<endl;
    cin.getline(aSalida,50,'\n');

    cout<<"Semilla"<<endl;
    cin>>seed;

    cout<<"Metodo de codificacion"<<endl;
    cin>>metodo;

    try{

        fin.open(aFuente);                   //abre el archivo para lectura
        if(!fin.is_open()){
            throw '1';
        }

        while(fin.good()){                  //lee caracter a caracter hasta el fin del archivo
            char temp=fin.get();
            if(fin.good()){
                datos[i]=temp;             //Asigna cada caracter leido a la cadena de caracteres
                i++;
            }

        }
        datos[i] = '\0';                   // agrega el caracter nulo al final de la cadena
        fin.close();                        //Cierra el archivo de lectura.


    }
    catch (char c){
        cout<<"Error # "<<c<<": ";
        if(c=='1'){
            cout<<"Error al abrir el archivo para lectura."<<endl;
        }
    }
    catch (...){
        cout<<"Error no definido"<<endl;
    }


    char cadenaBinaria[i * numBits + 1];                            // tamaño suficiente para almacenar los bits +1 para el caracter nulo
    int pos = 0;

    for(int i=0; datos[i] != '\0'; i++) {                          //convierte las letras en bits y luego los almacena en cadenaBinaria
        std::bitset<numBits> bits(datos[i]);
        std::string strBits = bits.to_string();

        for(int j=0; j < numBits; j++) {
            cadenaBinaria[pos++] = strBits[j];
        }
    }

    cadenaBinaria[pos] = '\0';                                      // agrega el caracter nulo al final de la cadena

    int tamCadena = sizeof(cadenaBinaria)-1;                      //tamaño del arreglo que contiene los bits
    int cantGrupos = tamCadena/seed;                            //cantidad de grupos de bits

    if(metodo==1){
        int temp = contCerosUnos(cadenaBinaria, seed);              //contiene mayor cantidad de numeros, 0=iguales, 1+cantidad 0, 2+cantidad 1
        int temp2 = 0;                                              //almacena temp para guardar la informacion del siguiente grupo sin perder la anterior

        invertBits(cadenaBinaria, seed, 1);                         //invierte el primer grupo de bits

        for(int i=1;i<=cantGrupos;i++){                             //itera todos los grupos llamando la funcion 'invertBits' para invertir los bits necesarios segun el caso de cada grupo
            temp2 = temp;
            temp = contCerosUnos(&cadenaBinaria[i*seed], seed);
            if(temp2==0){
                invertBits(&cadenaBinaria[i*seed],seed,1);          //se invierten todos los bits
            }
            else if(temp2==1){
                invertBits(&cadenaBinaria[i*seed],seed,2);          //se invierten cada 2 bits
            }
            else if(temp2==2){
                invertBits(&cadenaBinaria[i*seed],seed,3);          //se invierten cada 3 bits
            }
        }
    }

    if(metodo==2){
        for(int i=0;i<=cantGrupos-1;i++){                  //itera todos los grupos para desplazarlos a la derecha con la funcion 'desBits'
            desBits(&cadenaBinaria[i*seed],seed);
        }
        if(tamCadena%seed!=0 && tamCadena%seed!=1){        //en el caso de una cadena que no se pueda dividir en grupos de bits exactos se hace un grupo con los bits sobrantes y se desplazan
            int residuo = tamCadena%seed;
            desBits(&cadenaBinaria[tamCadena-residuo],residuo);
        }
    }

    try{

        fout.open(aSalida);                      //abre el archivo para escritura
        if(!fout.is_open()){
            throw '1';
        }
        fout<<cadenaBinaria;                     //escribe los bits
        fout.close();                            //cierra el archivo
    }
    catch (char c){
        cout<<"Error # "<<c<<": ";
        if(c=='1'){
            cout<<"Error al abrir el archivo para escritura.\n";
        }
    }
    catch (...){
        cout<<"Error no definido\n";
    }

    return 0;
}
