#include <iostream>
#include <fstream>
#include <sstream>
#include <bitset>
#include <string>
#include <limits>
#include <vector>
#include <map>

using namespace std;

string desBitsDer(string cdn, int max){             //desplaza un bit a la derecha en un grupo de bits (cadena de bits,tamaño grupos)
    char p = cdn[max-1];
    if(max==2){                              //si el grupo es de dos bits se intercambian
        if(cdn[0]=='0'){
            cdn[1]='0';
        }
        else if(cdn[0]=='1'){
            cdn[1]='1';
        }
    }
    else{
        for(int i=max-1;i>=1;i--){
            if(cdn[i-1]=='0'){
                cdn[i]='0';
            }
            else if(cdn[i-1]=='1'){
                cdn[i]='1';
            }
        }
    }
    cdn[0] = p;

    return cdn;
}

string desBitsIzq(string cdn, int max){             //desplaza un bit a la izquierda en un grupo de bits (cadena de bits,tamaño grupos)
    char p = cdn[0];
    if(max==2){                              //si el grupo es de dos bits se intercambian
        if(cdn[1]=='0'){
            cdn[0]='0';
        }
        else if(cdn[1]=='1'){
            cdn[0]='1';
        }
    }
    else{
        for(int i=0;i<=max-1;i++){
            if(cdn[i+1]=='0'){
                cdn[i]='0';
            }
            else if(cdn[i+1]=='1'){
                cdn[i]='1';
            }
        }
    }
    cdn[max-1] = p;

    return cdn;
}

string oDatos(string nFile){            //obtiene los datos de un archivo de texto y los devuelve desencriptados (nombre archivo de lectura)
    ifstream fin;                    //stream de entrada, lectura
    string cadenaBinaria;
    int seed = 4;

    try{

        fin.open(nFile);                   //abre el archivo para lectura
        if(!fin.is_open()){
            throw '1';
        }

        while(fin.good()){              //lee caracter a caracter hasta el fin del archivo
            char temp=fin.get();
            if(fin.good()){
                cadenaBinaria += temp;     //Asigna cada caracter leido a la cadena de caracteres
            }
        }
        fin.close();                //Cierra el archivo de lectura.


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

    int tamCadena = cadenaBinaria.size();                      //tamaño del arreglo que contiene los bits
    int cantGrupos = tamCadena/seed;                            //cantidad de grupos de bits

    string tempCad;

    for(int i=0;i<=cantGrupos-1;i++){                  //itera todos los grupos para desplazarlos a la izquierda con la funcion 'desBits'
        tempCad = desBitsIzq(cadenaBinaria.substr(i*seed,seed),seed);
        cadenaBinaria.replace(i*seed,seed,tempCad);
    }
    if(tamCadena%seed!=0 && tamCadena%seed!=1){        //en el caso de una cadena que no se pueda dividir en grupos de bits exactos se hace un grupo con los bits sobrantes y se desplazan
        int residuo = tamCadena%seed;
        tempCad = desBitsIzq(cadenaBinaria.substr(tamCadena-residuo,residuo),residuo);
        cadenaBinaria.replace(tamCadena-residuo,residuo,tempCad);
    }

    string datos;

    for (int i = 0; i < tamCadena; i += 8) { // Avanzamos de 8 en 8 caracteres
        int num = 0;
        for (int j = 0; j < 8; j++) {
            num = num * 2 + (cadenaBinaria[i + j] - '0');
        }
        datos += static_cast<char>(num);
    }
    return datos;
}

void aDatos(string nFile,string datos,int modo){            //encripta los datos y los almacena en un archivo (nombre archivo de escritura, datos a almacenar,modo:0==sobreescribir,1==añadir)
    ofstream fout;                   //stream de salida, escritura
    string cadenaBinaria;
    int seed = 4;
    const int numBits = 8;

    for(int i=0;datos[i]!='\0';i++) {
        std::bitset<numBits> bits(datos[i]);
        cadenaBinaria += bits.to_string();
    }

    int tamCadena = cadenaBinaria.size();                      //tamaño del arreglo que contiene los bits
    int cantGrupos = tamCadena/seed;                            //cantidad de grupos de bits

    string tempCad;

    for(int i=0;i<=cantGrupos-1;i++){                  //itera todos los grupos para desplazarlos a la derecha con la funcion 'desBits'
        tempCad = desBitsDer(cadenaBinaria.substr(i*seed,seed),seed);
        cadenaBinaria.replace(i*seed,seed,tempCad);
    }
    if(tamCadena%seed!=0 && tamCadena%seed!=1){        //en el caso de una cadena que no se pueda dividir en grupos de bits exactos se hace un grupo con los bits sobrantes y se desplazan
        int residuo = tamCadena%seed;
        tempCad = desBitsDer(cadenaBinaria.substr(tamCadena-residuo,residuo),residuo);
        cadenaBinaria.replace(tamCadena-residuo,residuo,tempCad);
    }

    try{
        if(modo == 0){
            fout.open(nFile, ios::app);         //abre el modo append
        }
        else if(modo == 1){
            fout.open(nFile);                   //abre el archivo para escritura
        }
        if(!fout.is_open()){
            throw '1';
        }
        fout<<cadenaBinaria;                     //escribe los bits
        fout.close();                            //cierra el archivo
    }
    catch (char c){
        cout<<"Error # "<<c<<": ";
        if(c=='1'){
            cout<<"Error al abrir el archivo para escritura."<<endl;
        }
    }
    catch (...){
        cout<<"Error no definido\n";
    }
}

string getNumero() {                      //comprueba que el valor introducido por el usuario sea un numero
    string entrada;
    bool esNumero = true;

    do {
        esNumero = true; // Reiniciar bool a true
        getline(cin, entrada);

        for (char c : entrada) {        //comprueba que cada caracter sea un numero
            if (!isdigit(c)) {
                esNumero = false;
                break;
            }
        }

        if (!esNumero) {
            cout << "Introduzca un numero valido: ";
        }

    } while (!esNumero);

    return entrada;
}

int select(int no){                     //pide un numero al usuario para navegar a travez de menus (numero de opciones)
    int selUser = 0;
    string entrada;
    bool esValido = false;

    while (!esValido) {
        cin >> entrada;

        cin.clear(); // limpia el estado de error
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // descarta el resto de la línea

        try {                                       //si el numero introducido no se puede convertir a int pide al usuario introducir el tipo de dato correcto
            selUser = stoi(entrada);
            if(selUser > 0 || selUser <= no){
                esValido = true;
            }
        }
        catch (const invalid_argument& e) {
            cerr<<"Introduzca solo numeros enteros."<<endl;
        }
    }
    return selUser;
}

int main()
{
    int eleccionMP = 0;         // eleccion del menu principal

    while(true){        //menu principal
        cout<<"Cajero"<<endl<<"1. Acceder como usuario administrador"<<endl<<"2. Acceder como usuario corriente"<<endl<<"3. Salir"<<endl;
        eleccionMP = select(3);

        if(eleccionMP == 1){                //sesion de usuario administrador
            string clave;
            string clavesudo = oDatos("sudo.txt");        //extrae la informacion de la clave del archivo sudo.txt

            cout<<"ingrese la clave de administrador"<<endl;
            cin>>clave;

            if(clave==clavesudo){
                int eleccionA = 0; //eleccion menu admin
                cout<<"Bienvenido Administrador"<<endl;

                while(true){            //menu admin
                    cout<<"1. Registrar usuario"<<endl<<"2. salir"<<endl;
                    eleccionA = select(2);

                    if(eleccionA == 1){                                     //pide los datos de el usuario para registrarlo
                        string user;
                        string entrada;

                        cout<<"Introduzca los siguientes datos del usuario:"<<endl<<"Cedula:";
                        cin.clear();
                        entrada = getNumero();
                        user += entrada;
                        user += ";";

                        cout<<"Clave: ";
                        getline(cin,entrada);
                        user += entrada;
                        user += ";";

                        cout<<"Saldo: ";
                        entrada = getNumero();
                        user += entrada;
                        user += "\n";

                        aDatos("userData",user,0);                    //agrega los datos del nuevo usuario al archivo userdata
                        cout<<"El usuario ha sido registrado"<<endl;
                    }
                    else if(eleccionA == 2){
                        break;
                    }
                }
            }
            else{
                cout<<"la clave es incorrecta"<<endl;
            }
        }
        else if(eleccionMP == 2){           //sesion de usuario corriente

            string datos = oDatos("userData");
            vector<map<string, string>> usuarios;       //contiene los datos de los usuarios ordenados

            size_t pos = 0;
            string linea;
            while ((pos = datos.find("\n")) != string::npos) { // divide la cadena en cada línea
                linea = datos.substr(0, pos);
                datos.erase(0, pos + 1);

                pos = 0;
                vector<string> partes;
                while ((pos = linea.find(";")) != string::npos) { // divide cada línea en sus partes
                    partes.push_back(linea.substr(0, pos));
                    linea.erase(0, pos + 1);
                }
                partes.push_back(linea); // la ultima parte no tiene ';'

                map<string, string> usuario;   //asigna cada parte a su mapa correspondiente
                usuario["cedula"] = partes[0];
                usuario["clave"] = partes[1];
                usuario["saldo"] = partes[2];
                usuarios.push_back(usuario);
            }

            string cedulaUser;        //cedula introducida por el usuario
            string claveUser;         //clave introducida por el usuario
            int cUser = -1;            //indice en el vector del usuario que inicio sesion
            int nUsuarios = usuarios.size()-1; //numero de usuarios registrados

            cout<<"Cedula: ";
            cin.clear();
            cedulaUser = getNumero();

            cout<<"Clave: ";
            getline(cin,claveUser);

            for(int i=0;i<=nUsuarios;i++){                    //busca el numero de cedula en el vector 'usuarios'
                if(usuarios[i]["cedula"] == cedulaUser){
                    cUser = i;          //asigna la posicion de la cedula en el vector
                    break;
                }
            }

            string claveTemp = "none";   //almacena la clave de la cedula para compararla con la introducida por el usuario

            if(cUser!=-1){                              //si la cedula existe asigna la clave de la cedula correspondiente a la variable
                claveTemp = usuarios[cUser]["clave"];
            }

            if(cUser==-1){
                cout<<"La cedula no se encuentra registrada"<<endl;
            }
            else if(claveTemp==claveUser){          //compara la clave de la cedula almacenada en el archivo con la introducida por el usuario
                int eleccionMU = 0; // eleccion menu usuario
                int saldoU = stoi(usuarios[cUser]["saldo"]);    //convierte el saldo del usuario en int para modificarlo

                saldoU -= 1000;     //realiza el cobro de los 1000 COP por ingresar al cajero

                cout<<"Bienvenido"<<endl;

                cout<<"1. Consultar saldo"<<endl<<"2. Retirar dinero"<<endl;
                eleccionMU = select(2);     //menu usuario corriente

                if(eleccionMU == 1){
                    cout<<"Saldo disponible: "<<saldoU<<" COP"<<endl;       //muestra el saldo del usuario
                }
                else if(eleccionMU == 2){
                    int saldoR = 0; //almacena el valor a retirar introducido por el usuario
                    while(true){
                        cout<<"ingrese la cantidad que desea retirar: ";
                        saldoR = stoi(getNumero());
                        if(saldoR > saldoU){
                            cout<<"No hay suficiente dinero en la cuenta"<<endl;
                        }
                        else if(saldoR < 0){
                            cout<<"No se puede retirar saldo negativo"<<endl;
                        }
                        else{
                            saldoU -= saldoR;       //retira el saldo
                            cout<<"Dinero retirado satisfactoriamente"<<endl;
                            break;
                        }
                    }
                }
                usuarios[cUser]["saldo"] = to_string(saldoU);       //convierte el saldo en string y lo almacena en la variable correspondiente del vector
                string datosAct;
                for(const auto& usuario : usuarios) {           //convierte los datos almacenados en el vector en un string para almacenarlos en el archivo 'userData'
                    datosAct += usuario.at("cedula") + ";" + usuario.at("clave") + ";" + usuario.at("saldo") + "\n";
                }
                aDatos("userData",datosAct,1); //reemplaza los datos antiguos por los cambios realizados
            }
            else{
                cout<<"cedula o clave incorrecta"<<endl;
            }
        }
        else if(eleccionMP == 3){
            break;
        }
    }
    return 0;
}
