#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>

using namespace std;

void invertBits(char *cdn, int max, int salto){             //invierte los bits(cadena de bits, tamaño grupos, cada cuantos bits es invierte)
    if(salto==1){                                  //si el parametro salto == 1 se invierten todos los bits del grupo
        for(int i=0;i<=max-1;i++){
            if(*(cdn+i)=='0'){
                *(cdn+i)='1';
            }
            else if(*(cdn+i)=='1'){
                *(cdn+i)='0';
            }
        }
    }
    else{                                         //si el parametro salto != 1 se invierten tomando en cuenta el valor de salto
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

void desBits(char *cdn, int max){             //desplaza un bit a la izquierda en un grupo de bits (cadena de bits,tamaño grupos)
    char p = *cdn;
    if(max==2){                              //si el grupo es de dos bits se intercambian
        if(*(cdn+1)=='0'){
            *cdn='0';
        }
        else if(*(cdn+1)=='1'){
            *cdn='1';
        }
    }
    else{
        for(int i=0;i<max-1;i++){
            if(*(cdn+i+1)=='0'){
                *(cdn+i)='0';
            }
            else if(*(cdn+i+1)=='1'){
                *(cdn+i)='1';
            }
        }
    }
    *(cdn+max-1) = p;
}

int main()
{
    ifstream fin;                    //stream de entrada, lectura
    ofstream fout;                   //stream de salida, escritura
    char cadenaBinaria[8000];
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
                cadenaBinaria[i]=temp;             //Asigna cada caracter leido a la cadena de caracteres
                i++;
            }

        }
        cadenaBinaria[i] = '\0';            // agrega el caracter nulo al final de la cadena
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

    int tamCadena = strlen(cadenaBinaria);                      //tamaño del arreglo que contiene los bits
    int cantGrupos = tamCadena/seed;                            //cantidad de grupos de bits

    if(metodo==1){
        int temp = contCerosUnos(cadenaBinaria, seed);              //contiene mayor cantidad de numeros, 0=iguales, 1+cantidad 0, 2+cantidad 1

        invertBits(cadenaBinaria, seed, 1);                         //invierte el primer grupo de bits

        for(int i=1;i<=cantGrupos;i++){                             //itera todos los grupos llamando la funcion 'invertBits' para invertir los bits necesarios segun el caso de cada grupo
            temp = contCerosUnos(&cadenaBinaria[i*seed-seed], seed);
            if(temp==0){
                invertBits(&cadenaBinaria[i*seed],seed,1);          //se invierten todos los bits
            }
            else if(temp==1){
                invertBits(&cadenaBinaria[i*seed],seed,2);          //se invierten cada 2 bits
            }
            else if(temp==2){
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

    char datos[tamCadena/8+1];
    int e = 0;

    for(int i = 0; i < tamCadena; i += 8) {      //itera a través de la cadena de caracteres
        int num = 0;
        for(int j=0;j<8;j++) {                   //convierte la secuencia de bits en un número entero
            num = num * 2 + (cadenaBinaria[i+j] - '0');
        }

        datos[e] = static_cast<char>(num);       //convierte el número entero en una letra y lo agregamos a la cadena de letras
        e++;
    }

    datos[e] = '\0';

    try{
        fout.open(aSalida);                      //abre el archivo para escritura
        if(!fout.is_open()){
            throw '1';
        }
        fout<<datos;                             //escribe la palabra
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
