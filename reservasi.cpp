#include <iostream>
#include <windows.h>
#include <fstream>
#include <stdio.h>
#include <conio.h>
#include <string>
#include <random>
#include <chrono>
#include <iomanip>
using namespace std;

struct flight_schedule{
    int date;
    int month;
    int year;
    int hour;
    int minute;
};
struct flight_ticket{
    int ticket_id;
    long long price;
    string airline;
    string category;
    string airportFrom;
    string IATA_from;
    string airportTo;
    string IATA_to;
    string flight;  
    string status;
    flight_schedule scheduleDate;
};
struct data_penumpang{
    long long id;
    string name;
    flight_ticket ticket;
};

struct akun{
    string username;
    string password;
    data_penumpang passenger[500];
    int passengerCount;
    long long saldo;
}user[500];

//Date
// Dapatkan waktu saat ini
auto now = chrono::system_clock::now();
// Konversi waktu saat ini menjadi waktu kalender
time_t t = chrono::system_clock::to_time_t(now);
// Konversi waktu kalender menjadi struktur waktu tm
tm* now_time = localtime(&t);


//Global variable
fstream file;
ofstream ofs;
ifstream ifs;

// akun
int pilihMenu; // Menu dll
int userCount, indexUser, indexPenumpang; // Indexing
bool loopBack, same, adaAkun; // akun

//ticketing
int passengerTemp, passengerNum, inputReservation;
bool lanjutReservasi; //tiketing
char is_ulang;

//Declare Void System
void login();
void bikinAkun(fstream &file);
void hapusAkun();
void top_up();
void pesenTiket();
void maskapai();
void kategoriKelas();
void flight();
void harga();
void departure();
void arrival();
void date();
void output();
void bayar();
//Void Eksport Import
void eksportAkunIndex(ofstream &ofs);
void importAkunIndex(ifstream &ifs);
void eksportAkunUsername(ofstream &ofs);
void importAkunData(ifstream &ifs);
void eksportAkunData(ofstream &ofs);
void eksportPassengerCount(ofstream &ofs); //Import index ke txt
void importPassengerCount(ifstream &ifs); //Import index ke txt
void eksportPassengerTicket(ofstream &ofs);
void importPassengerTicket(ifstream &ifs);

int loginPage(){
    importAkunIndex(ifs);
    eksportAkunIndex(ofs);
    importAkunData(ifs);
    akun user_temp;
    // sorting account
    for (int i = 0; i < userCount-1; i++) {
        for (int j = i+1; j < userCount; j++) {
            if (user[j].username < user[i].username) {
                user_temp = user[j];
                user[j] = user[i];
                user[i] = user_temp;
            }
        }
    }
    eksportAkunData(ofs);
    system("cls");
    cout << "\t\t" << string(26, '-') << endl;
    cout << "\t\t\tLogin Page\n";
    cout << "\t\t" << string(26, '-') << endl << endl;
    cout << "1. Login\n";
    cout << "2. Register\n";
    cout << "3. Delete Account\n";
    cout << "4. Exit";
    cout << "\n>> ";cin>>pilihMenu;    
    cin.ignore();
    return pilihMenu;
}

int mainMenu(){
    system("cls");
    cout << "\t\t" << string(51, '-') << endl;
    cout << "\t\t\tNational Flight Ticket Reservation\n";
    cout << "\t\t" << string(51, '-') << endl << endl;
    cout << user[indexUser].username << " || " << "Balance : " << user[indexUser].saldo << string(60 - (user[indexUser].username.length() + to_string(user[indexUser].saldo).length()-7 ), ' ');
    cout << now_time->tm_mday << "/" << (now_time->tm_mon + 1) << "/"<< (now_time->tm_year + 1900) << endl;
    cout << string(90, '-');
    //import index user
    importPassengerCount(ifs);
    passengerNum = user[indexUser].passengerCount;
    indexPenumpang =  user[indexUser].passengerCount;
    importPassengerTicket(ifs);
    harga();
    cout << "\nYou have " << indexPenumpang << " reserved flight ticket\n\n";
    cout << "Menu : \n";
    cout << string(16, '-') << endl;
    cout << "1. Order Ticket\n";
    cout << "2. List Ticket\n";
    cout << "3. Top Up\n";
    cout << "4. Log out";
    cout << "\n>> ";cin>>pilihMenu; 
    cin.ignore();
    return pilihMenu;
}

int main(){
    int Login;
    enum option{LOGIN = 1, REGISTER, HAPUS, EXIT};
    do{
        Login = loginPage();
        switch (Login){
        case LOGIN:
            login();
            if(adaAkun) break;
        case REGISTER:
            bikinAkun(file);
            break;
        case HAPUS:
            hapusAkun();
            break;
        case EXIT:
            cout << "\nThank you. See You Again.";
            getch();
            return(0);
        default:
            cout << "\nInvalid input menu. Try again . . . ";
            getch();
            break;
        }
    } while (Login != EXIT);
    return 0;
}

void login(){
    //Local Var
    bool berhasilLogin;
    int userLogin, Program_Utama;
    string passLogin;
    do{
        system("cls");
        cout << "\t\t" << string(26, '-') << endl;
        cout << "\t\t\tLogin Page\n";
        cout << "\t\t" << string(26, '-') << endl << endl;
        if(userCount == 0){
            cout << "You don't have an account, at least make one! \n";
            cout << "Go to registration page . . .  ";
            getch();
            adaAkun = false; // gada break, lanjut registrasi
        }
        else{
            adaAkun = true;
            cout << "List Account\n";
            cout << string(12, '-') << endl;

            //show registered account
            for(int i = 0; i <= userCount; i++){
                if(i < userCount) cout << i + 1 << ". " << user[i].username << endl;
                else if(i == userCount) cout << endl<< i + 1 << ". Back\n";
            }
            cout << "\nChoose an account to login.\n>> "; cin >> userLogin;
            cin.ignore();
            for(int i = 0; i <= userCount; i++){
                if(userLogin - 1 == i){
                    cout << "\n\nLogin to " << user[i].username <<"'s account. Please enter the password.";
                    while(passLogin != "back"){
                        cout << "\n\nPassword : "; getline(cin, passLogin);
                        if(passLogin == user[i].password){ //berhasil login
                            indexUser = i;
                            berhasilLogin = true;
                            break; //break dari while
                        }
                        else{
                            cout << "Password is wrong, Try again or Input \"back\" to stop trying. . .\n";
                            loopBack = true;
                        }
                    }
                    loopBack = false; // kalo berhasil login
                    break; // break dari for
                }
                if(userLogin - 1 == userCount){ //back
                    berhasilLogin = false;
                    loopBack = false;
                    break;
                }
                else if(userLogin - 1 > userCount || userLogin - 1 < 1 || cin.fail()){
                    cout << "\nInvalid input. Choose the available account. . . ";
                    getch();
                    berhasilLogin = false;
                    loopBack = true;
                    break; //break dari for
                }
            }
        }
    }while(loopBack);

    if(adaAkun){
                if(berhasilLogin == true){
                    do{
                        Program_Utama = mainMenu();
                        switch (Program_Utama){
                        case 1:
                            pesenTiket();
                            break; // break case
                        case 2:
                            output();
                            break; //Break case
                        case 3:
                            top_up();
                            getch();      
                            break;       
                        case 4:
                            cout << "\nBack to Login Page. . . ";
                            getch();  
                            berhasilLogin = false;
                            break;  
                        default:
                            cout << "\nMenu unavailable, try again. . .";
                            getch();
                            break;
                        }
                    }while (Program_Utama != 4);
                }
            } 
}

void bikinAkun(fstream &file){
    do{
        same = false;
        system("cls");
        cout << "\t\t" << string(34, '-') << endl;
        cout << "\t\t\tRegistration Page\n";
        cout << "\t\t" << string(34, '-') << endl << endl;
        cout << "Username : "; getline(cin, user[userCount].username);

        ifs.open("Database/Ticket/ string " + user[userCount].username +  ".txt"); //check Uname
        //if account udah ada, maka
        if(ifs.is_open()){ 
                cout << "\nUsername " << user[userCount].username << " is already used. Please use another username. . . ";
                same = true;
                getch();
            ifs.close();
        }
    }while(same);
    do{
        cout << "Password : "; getline(cin, user[userCount].password);
        if(user[userCount].password == "back") cout << "Please don't use \"back\" as a password. Try again.\n";
    }while (user[userCount].password == "back" || user[userCount].password == "");
    // bikin baru data
    ofs.open("Akun/data.txt", ios::app);
        ofs << user[userCount].saldo << " "
            << user[userCount].username << endl
            << user[userCount].password << endl;
    ofs.close();
    //bikin file username.txt
    ofs.open("Database/Ticket/ string " + user[userCount].username +  ".txt");
    ofs.close();
    ofs.open("Database/Ticket/ int " + user[userCount].username +  ".txt");
    ofs.close();
    userCount ++;
    eksportAkunIndex(ofs);

    akun user_temp;
    // sorting account
    for (int i = 0; i < userCount-1; i++) {
        for (int j = i+1; j < userCount; j++) {
            if (user[j].username < user[i].username) {
                user_temp = user[j];
                user[j] = user[i];
                user[i] = user_temp;
            }
        }
    }
    eksportAkunData(ofs);
    cout << "\n\nLoading.";
    Sleep(1000);
    cout << ".";
    Sleep(1000);
    cout << ".";
    Sleep(1000);
    cout << "\n\nRegister Success!\n";
    cout << "Back to Login Page. . .";
    getch();
}

void hapusAkun(){
    int userDelete;
    string passDelete, delFileS;
    bool berhasilDelete;
    const char *delFile;
    do{
        system("cls");
        cout << "\t\t" << string(34, '-') << endl;
        cout << "\t\t\t   Delete Account\n";
        cout << "\t\t" << string(34, '-') << endl << endl;
        if(userCount == 0){
            cout << "You don't have an account, Nothing can be deleted.";
            getch();
        }
        else{
            cout << "List Account\n";
            cout << string(12, '-') << endl;

            //show registered account
            for(int i = 0; i <= userCount; i++){
                if(i < userCount) cout << i + 1 << ". " << user[i].username << endl;
                else if(i == userCount) cout << endl<< i + 1 << ". Back\n";
            }
            cout << "\nChoose an account to delete.\n>> "; cin >> userDelete;
            cin.ignore();
            for(int i = 0; i <= userCount; i++){
                if(userDelete - 1 == i){
                    cout << "\n\nDeleting " << user[i].username <<"'s account. Please enter the password.";
                    while(passDelete != "back"){
                        cout << "\n\nPassword : "; getline(cin, passDelete);
                        if(passDelete == user[i].password){ //berhasil delete
                            indexUser = i;
                            berhasilDelete = true;
                            break; //break dari while
                        }
                        else{
                            cout << "Password is wrong, is that you?\n";
                            cout << "Try again or Input \"back\" to stop trying. . .\n";
                            loopBack = true;
                        }
                    }
                    loopBack = false; // kalo berhasil delete
                    break; // break dari for
                }
                if(userDelete - 1 == userCount){ //back
                    berhasilDelete = false;
                    loopBack = false;
                    break;
                }
                else if(userDelete - 1 > userCount || userDelete - 1 < 1 || cin.fail()){
                    cout << "\nInvalid input. Choose the available account. . . ";
                    getch();
                    berhasilDelete = false;
                    loopBack = true;
                    break; //break dari for
                }
            }
        }
    }while(loopBack);

    if(berhasilDelete){
        
        delFileS = "Database/Ticket/ int " + user[indexUser].username + ".txt";
        delFile = delFileS.c_str();
        int a = remove(delFile);

        delFileS = "Database/Ticket/ string " + user[indexUser].username + ".txt";
        delFile = delFileS.c_str();
        a = remove(delFile);

        delFileS = "Database/index penumpang/index " + user[indexUser].username + ".txt";
        delFile = delFileS.c_str();
        a = remove(delFile);

        swap(user[indexUser], user[userCount-1]);
        userCount--;

        eksportAkunData(ofs);
        eksportAkunIndex(ofs);
        cout << "\n\nLoading.";
        Sleep(1000);
        cout << ".";
        Sleep(1000);
        cout << ".";
        Sleep(1000);
        cout << "\n\nSuccess!\nDeleting " << user[userCount].username << "'s account.\n";
        cout << "Back to Login Page. . .";
        getch();
    }
}

void top_up(){
    long long top_up;
    string saldo;
    system("cls");
    cout << "\t\t" << string(26, '-') << endl;
    cout << "\t\t\t  Top Up\n";
    cout << "\t\t" << string(26, '-') << endl << endl;
    cout << user[indexUser].username << " || " << "Balance : " << user[indexUser].saldo << endl;
    cout << string(14 + (user[indexUser].username.length() + to_string(user[indexUser].saldo).length() ), '-');
    cout << "\nTop Up amount : ";
    cin >> top_up;
    user[indexUser].saldo += top_up;
    cout << "Loading.";
    Sleep(1000);
    cout << ".";
    Sleep(1000);
    cout << ".";
    Sleep(1000);

    system("cls");
    cout << "\t\t" << string(26, '-') << endl;
    cout << "\t\t\t  Top Up\n";
    cout << "\t\t" << string(26, '-') << endl << endl;
    cout << user[indexUser].username << " || " << "Balance : " << user[indexUser].saldo << endl;
    cout << string(14 + (user[indexUser].username.length() + to_string(user[indexUser].saldo).length()), '-');
    cout << "\n\nTop Up Success!";
    cout << "\nBack to Login Page. . .";

    eksportAkunData(ofs);

}

void pesenTiket(){
    //locar var
    bool fixRegistrasi = true;
    lanjutReservasi = true;
    while(lanjutReservasi){
            system("cls");
            cout << "\n\nAmount of book tickets ?\n>> ";cin>>passengerTemp;
            cin.ignore();
            passengerNum += passengerTemp;
            while(indexPenumpang < passengerNum){
                system("cls");
                cout << "Passenger Number : " << indexPenumpang + 1 << endl;
                cout << string(20, '-') << endl;
                user[indexUser].passenger[indexPenumpang].ticket.ticket_id = ((indexUser+1) * 100) + indexPenumpang + 1;
                cout << "Ticket ID    : " << user[indexUser].passenger[indexPenumpang].ticket.ticket_id << endl;
                cout << "Passenger ID : "; cin>> user[indexUser].passenger[indexPenumpang].id;
                cin.ignore();
                cout << "Name         : ";
                getline(cin, user[indexUser].passenger[indexPenumpang].name);
                maskapai();
                kategoriKelas();
                harga();
                flight();
                do{
                    departure();
                    arrival();
                    if(user[indexUser].passenger[indexPenumpang].ticket.airportFrom == user[indexUser].passenger[indexPenumpang].ticket.airportTo){
                        cout << "\nOrigin and Destination place is same. Please try again.";
                        getch();
                    }
                }while(user[indexUser].passenger[indexPenumpang].ticket.airportFrom == user[indexUser].passenger[indexPenumpang].ticket.airportTo);
                
                date();

                //ticket status
                user[indexUser].passenger[indexPenumpang].ticket.status = "Booked";
                system("cls");
                cout << "\t\t\t Booking ticket information\n\n";
                cout << "Ticket ID   : " << user[indexUser].passenger[indexPenumpang].ticket.ticket_id << endl;
                cout << "ID          : " << user[indexUser].passenger[indexPenumpang].id << endl;
                cout << "Name        : " << user[indexUser].passenger[indexPenumpang].name <<  endl;
                cout << "Airline     : " << user[indexUser].passenger[indexPenumpang].ticket.airline << endl;
                cout << "FLight      : " << user[indexUser].passenger[indexPenumpang].ticket.flight << endl;
                cout << "Category    : " << user[indexUser].passenger[indexPenumpang].ticket.category << endl;
                cout << "Origin      : " << user[indexUser].passenger[indexPenumpang].ticket.IATA_from << endl;
                cout << "Departure   : " << user[indexUser].passenger[indexPenumpang].ticket.airportFrom << endl;
                cout << "Destination : " << user[indexUser].passenger[indexPenumpang].ticket.IATA_to << endl;
                cout << "Arrival     : " << user[indexUser].passenger[indexPenumpang].ticket.airportTo << endl; 
                cout << "Date        : " ; if(user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.date < 10) cout << "0";
                                                cout << user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.date << "/" ;
                                            if(user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.month < 10) cout << "0";
                                                cout << user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.month << "/"
                                                    << user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.year << endl;
                cout << "Time        : " ;  if(user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.hour < 10) cout << "0";
                                                cout << user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.hour << ":" ;
                                            if(user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.minute < 10) cout << "0";
                                                cout << user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.minute << endl;
                cout << "Price       : IDR. " << user[indexUser].passenger[indexPenumpang].ticket.price << endl;
                cout << endl << user[indexUser].passenger[indexPenumpang].ticket.status << endl << endl;       
                do{
                    cout << "\nWant to change the reservation ? (y/n)\n>> "; cin >> is_ulang;
                    if(is_ulang == 'y' || is_ulang == 'Y'){
                        fixRegistrasi = false;
                        break; //break do while
                    }
                    else if(is_ulang == 'n' || is_ulang == 'N'){
                        fixRegistrasi = true;
                        break; ///break do while
                    }
                    else loopBack = true;
                }while(loopBack);

                if(fixRegistrasi){
                    eksportPassengerTicket(ofs);
                    indexPenumpang ++;
                    eksportPassengerCount(ofs);
                } 
        }  
        if(passengerTemp > 1) cout << passengerTemp << " Booking tickets success added !\n"; 
        else cout << passengerTemp << " Booking ticket success added !\n"; 
        lanjutReservasi = false;
    }
}

void maskapai(){
    do{
        lanjutReservasi = true;
        system("cls");
        cout << "\n\nPilih Maskapai\n";
        cout << "1. Garuda Indonesia\n2. Lion Air\n3. Air Asia\n4. Citilink\n5. Batik Air\n\n>> ";cin>>inputReservation;
        switch (inputReservation){
            case 1:
                user[indexUser].passenger[indexPenumpang].ticket.airline = "Garuda Indonesia";
                break;
            case 2:
                user[indexUser].passenger[indexPenumpang].ticket.airline = "Lion Air";
                break;
            case 3:
                user[indexUser].passenger[indexPenumpang].ticket.airline = "Air Asia";
                break;
            case 4:
                user[indexUser].passenger[indexPenumpang].ticket.airline = "Citilink";
                break;
            case 5:
                user[indexUser].passenger[indexPenumpang].ticket.airline = "Batik Air";
                break;  
            default:
                cout << "Invalid Input, airline is unknown. Try Again.";
                lanjutReservasi = false;
                getch();
                system("cls");
                break;
            }
        }while(!lanjutReservasi);
}

void kategoriKelas(){
    do{
        lanjutReservasi = true;
        system("cls");
        cout << "\nWelcome to " << user[indexUser].passenger[indexPenumpang].ticket.airline << " flight ticket reservation menu.";
        cout << "\nClass  : " << endl;
        cout << "\t1. Economy\n\t2. Business\n\t3. First Class\n\t>> ";cin >> inputReservation;
        switch (inputReservation){
            case 1:
                user[indexUser].passenger[indexPenumpang].ticket.category = "Economy Class";
                break;
            case 2:
                user[indexUser].passenger[indexPenumpang].ticket.category = "Business Class";
                break;
            case 3: 
                user[indexUser].passenger[indexPenumpang].ticket.category = "First Class";
                break;
            default:
                cout << "Wrong input, class category is unknown. Try again.";
                getch();
                lanjutReservasi = false;
                system("cls");
                break;
        }
    }while(!lanjutReservasi);
}

void flight(){
    int flightCode;
    string nomor;
    flightCode = rand() % 198 + 101;
    nomor = to_string(flightCode);

    if(user[indexUser].passenger[indexPenumpang].ticket.airline == "Garuda Indonesia"){
        user[indexUser].passenger[indexPenumpang].ticket.flight = "GA-";
    }
    if(user[indexUser].passenger[indexPenumpang].ticket.airline == "Lion Air"){
        user[indexUser].passenger[indexPenumpang].ticket.flight = "JT-";
    }
    if(user[indexUser].passenger[indexPenumpang].ticket.airline == "Air Asia"){
        user[indexUser].passenger[indexPenumpang].ticket.flight = "QZ-";
    }
    if(user[indexUser].passenger[indexPenumpang].ticket.airline == "Citilink"){
        user[indexUser].passenger[indexPenumpang].ticket.flight = "QG-";
    }
    if(user[indexUser].passenger[indexPenumpang].ticket.airline == "Batik Air"){
        user[indexUser].passenger[indexPenumpang].ticket.flight = "ID-";
    }
    user[indexUser].passenger[indexPenumpang].ticket.flight += nomor;
}

void harga(){
    if(user[indexUser].passenger[indexPenumpang].ticket.airline == "Garuda Indonesia"){
        if(user[indexUser].passenger[indexPenumpang].ticket.category == "Economy Class")
            user[indexUser].passenger[indexPenumpang].ticket.price = 2350000;
        if(user[indexUser].passenger[indexPenumpang].ticket.category == "Business Class")
            user[indexUser].passenger[indexPenumpang].ticket.price = 3200000;
        if(user[indexUser].passenger[indexPenumpang].ticket.category == "First Class")
            user[indexUser].passenger[indexPenumpang].ticket.price = 5500000;
    }
    else if(user[indexUser].passenger[indexPenumpang].ticket.airline == "Lion Air"){
        if(user[indexUser].passenger[indexPenumpang].ticket.category == "Economy Class")
            user[indexUser].passenger[indexPenumpang].ticket.price = 1900000;
        if(user[indexUser].passenger[indexPenumpang].ticket.category == "Business Class")
            user[indexUser].passenger[indexPenumpang].ticket.price = 2600000;
        if(user[indexUser].passenger[indexPenumpang].ticket.category == "First Class")
            user[indexUser].passenger[indexPenumpang].ticket.price = 4500000;
    }
    else if(user[indexUser].passenger[indexPenumpang].ticket.airline == "Air Asia"){
        if(user[indexUser].passenger[indexPenumpang].ticket.category == "Economy Class")
            user[indexUser].passenger[indexPenumpang].ticket.price = 2000000;
        if(user[indexUser].passenger[indexPenumpang].ticket.category == "Business Class")
            user[indexUser].passenger[indexPenumpang].ticket.price = 2750000;
        if(user[indexUser].passenger[indexPenumpang].ticket.category == "First Class")
            user[indexUser].passenger[indexPenumpang].ticket.price = 4700000;
    }
    else if(user[indexUser].passenger[indexPenumpang].ticket.airline == "Citilink"){
        if(user[indexUser].passenger[indexPenumpang].ticket.category == "Economy Class")
            user[indexUser].passenger[indexPenumpang].ticket.price = 2200000;
        if(user[indexUser].passenger[indexPenumpang].ticket.category == "Business Class")
            user[indexUser].passenger[indexPenumpang].ticket.price = 2950000;
        if(user[indexUser].passenger[indexPenumpang].ticket.category == "First Class")
            user[indexUser].passenger[indexPenumpang].ticket.price = 5000000;
    }
    else if(user[indexUser].passenger[indexPenumpang].ticket.airline == "Batik Air"){
        if(user[indexUser].passenger[indexPenumpang].ticket.category == "Economy Class")
            user[indexUser].passenger[indexPenumpang].ticket.price = 19500000;
        if(user[indexUser].passenger[indexPenumpang].ticket.category == "Business Class")
            user[indexUser].passenger[indexPenumpang].ticket.price = 2500000;
        if(user[indexUser].passenger[indexPenumpang].ticket.category == "First Class")
            user[indexUser].passenger[indexPenumpang].ticket.price = 4750000;
    }

}

void departure(){
    do{
        lanjutReservasi = true;
        system("cls");
        cout<<"\nOrigin : "<<endl;
        cout<<"\t1. Sumatera\n\t2. Jawa dan Bali\n\t3. Kalimantan\n\t4. Sulawesi\n\t5. Nusa Tenggara\n\t6. Maluku\n\t7. Papua\n\t>> ";cin>>inputReservation;
        switch (inputReservation){
            case 1:
                cout<<"\t\t Bandara Sumatera\n";
                cout<<"1. Banda Aceh (BTJ)\n";
                cout<<"2. Lampung (TKG)\n";
                cout<<"3. Batam (BTH)\n";
                cout<<"4. Deli Serdang (KNO)\n";
                cout<<"5. Padang (PDG)\n";
                cout<<"6. Palembang (PLM)\n";
                cout<<"7. Pekanbaru (PKU)\n";
                cout<<"8. Bangka Belitung (TJQ)\n";
                cout<<"9. Tanjung Pinang (TNJ)\n";
                
                cout<<">> "; cin>>inputReservation;
                switch(inputReservation){
                    case 1:
                        user[indexUser].passenger[indexPenumpang].ticket.airportFrom = "Bandar Udara Internasional Sultan Iskandar Muda "; // CEK WEBSITE
                        user[indexUser].passenger[indexPenumpang].ticket.IATA_from = "Banda Aceh (BTJ)";
                        break;
                    case 2:
                        user[indexUser].passenger[indexPenumpang].ticket.airportFrom = "Bandar Udara Internasional Radin Inten II";
                        user[indexUser].passenger[indexPenumpang].ticket.IATA_from = "Bandar Lampung (TKG)";
                        break;
                    case 3:
                        user[indexUser].passenger[indexPenumpang].ticket.airportFrom = "Bandar Udara Internasional Hang Nadim";
                        user[indexUser].passenger[indexPenumpang].ticket.IATA_from = "Batam (BTH)";
                        break;
                    case 4:
                        user[indexUser].passenger[indexPenumpang].ticket.airportFrom = "Bandar Udara Internasional Kualanamu";
                        user[indexUser].passenger[indexPenumpang].ticket.IATA_from = "Deli Serdang (KNO)";
                        break;
                    case 5:
                        user[indexUser].passenger[indexPenumpang].ticket.airportFrom = "Bandar Udara Internasional Minangkabau";
                        user[indexUser].passenger[indexPenumpang].ticket.IATA_from = "Padang (PDG)";
                        break;
                    case 6:
                        user[indexUser].passenger[indexPenumpang].ticket.airportFrom = "Bandar Udara Internasional Sultan Mahmud Badarrudin";
                        user[indexUser].passenger[indexPenumpang].ticket.IATA_from = "Palembang (PLM)";
                        break;
                    case 7:
                        user[indexUser].passenger[indexPenumpang].ticket.airportFrom = "Bandar Udara Internasional Sultan Syarif Kasim II";
                        user[indexUser].passenger[indexPenumpang].ticket.IATA_from = "Pekanbaru (PKU)";
                        break;
                    case 8:
                        user[indexUser].passenger[indexPenumpang].ticket.airportFrom = "Bandar Udara Internasional H.A.S Hanandjoeddin";
                        user[indexUser].passenger[indexPenumpang].ticket.IATA_from = "Bangka Belitung (TJQ)";
                        break;
                    case 9:
                        user[indexUser].passenger[indexPenumpang].ticket.airportFrom = "Bandar Udara Internasional Raja Haji Fisabillah";
                        user[indexUser].passenger[indexPenumpang].ticket.IATA_from = "Tanjung Pinang (TNJ)";
                        break;
                    default:
                        cout<<"Wrong input, origin is undefined. Try again.";
                        lanjutReservasi = false;
                        getch();
                        break;
                }
                break;
            case 2:
                cout<<"\t\t Bandara Jawa dan Bali\n";
                cout<<"1. Bandung (BDO)\n";
                cout<<"2. Bali (DPS)\n";
                cout<<"3. Jakarta (HLP)\n";
                cout<<"4. Tangerang (CGK)\n";
                cout<<"5. Majalengka (KJT)\n";
                cout<<"6. Semarang (SMG)\n";
                cout<<"7. Sidoarjo (SUB)\n";
                cout<<"8. Boyolali (SOC)\n";
                cout<<"9. Yogyakarta (JOG)\n";
                cout<<"10. Kulonprogo (YIA)\n";
                cout<<">> "; cin>>inputReservation;
                switch(inputReservation){
                case 1:
                    user[indexUser].passenger[indexPenumpang].ticket.airportFrom = "Bandar Udara Internasional Husein Sastranegara "; // CEK WEBSITE
                    user[indexUser].passenger[indexPenumpang].ticket.IATA_from = "Bandung (BDO)";
                    break;
                case 2:
                    user[indexUser].passenger[indexPenumpang].ticket.airportFrom = "Bandar Udara Internasional I Gusti Ngurah Rai "; // CEK WEBSITE
                    user[indexUser].passenger[indexPenumpang].ticket.IATA_from = "Denpasar (DPS)";
                    break;
                case 3:
                    user[indexUser].passenger[indexPenumpang].ticket.airportFrom = "Bandar Udara Internasional Halim Perdanakusuma "; // CEK WEBSITE
                    user[indexUser].passenger[indexPenumpang].ticket.IATA_from = "Jakarta (HLP)";
                    break;
                case 4:
                    user[indexUser].passenger[indexPenumpang].ticket.airportFrom = "Bandar Udara Internasional Soekarno-Hatta "; // CEK WEBSITE
                    user[indexUser].passenger[indexPenumpang].ticket.IATA_from = "Tangerang (CGK)";
                    break;
                case 5:
                    user[indexUser].passenger[indexPenumpang].ticket.airportFrom = "Bandar Udara Internasional Kertajati"; // CEK WEBSITE
                    user[indexUser].passenger[indexPenumpang].ticket.IATA_from = "Majalengka (KJT)";
                    break;
                case 6:
                    user[indexUser].passenger[indexPenumpang].ticket.airportFrom = "Bandar Udara Internasional Jenderal Ahmad Yani"; // CEK WEBSITE
                    user[indexUser].passenger[indexPenumpang].ticket.IATA_from = "Semarang (SMG)";
                    break;
                case 7:
                    user[indexUser].passenger[indexPenumpang].ticket.airportFrom = "Bandar Udara Internasional Juanda"; // CEK WEBSITE
                    user[indexUser].passenger[indexPenumpang].ticket.IATA_from = "Sidoarjo (SUB)";
                    break;
                case 8:
                    user[indexUser].passenger[indexPenumpang].ticket.airportFrom = "Bandar Udara Internasional Adisumarmo "; // CEK WEBSITE
                    user[indexUser].passenger[indexPenumpang].ticket.IATA_from = "Boyolali (SOC)";
                    break;
                case 9:
                    user[indexUser].passenger[indexPenumpang].ticket.airportFrom = "Bandar Udara Internasional Adisucipto "; // CEK WEBSITE
                    user[indexUser].passenger[indexPenumpang].ticket.IATA_from = "Yogyakarta (JOG)";
                    break;
                case 10:
                    user[indexUser].passenger[indexPenumpang].ticket.airportFrom = "Bandar Udara Internasional Yogyakarta"; // CEK WEBSITE
                    user[indexUser].passenger[indexPenumpang].ticket.IATA_from = "Kulonprogo (YIA)";
                    break;
                default:
                    cout<<"Wrong input, origin is undefined. Try again.";
                    lanjutReservasi = false;
                    getch();
                    break;
                }
                break;
            case 3: 
                cout<<"\t\t Bandara Kalimantan\n";
                cout<<"1. Balikpapan (BPN)\n";
                cout<<"2. Banjarbaru (BDJ)\n";
                cout<<"3. Pontianak (PNK)\n";
                cout<<"4. Samarinda (AAP)\n";
                cout<<"5. Tarakan (TRK)\n";
                cout<<">> "; cin>>inputReservation;
                switch (inputReservation){
                    case 1:
                        user[indexUser].passenger[indexPenumpang].ticket.airportFrom = "Bandar Udara Internasional Sultan Aji Muhammad Sulaiman Sepinggan "; // CEK WEBSITE
                        user[indexUser].passenger[indexPenumpang].ticket.IATA_from = "Balikpapan (BPN)";
                        break;
                    case 2:
                        user[indexUser].passenger[indexPenumpang].ticket.airportFrom = "Bandar Udara Internasional Syamsudin Noor "; // CEK WEBSITE
                        user[indexUser].passenger[indexPenumpang].ticket.IATA_from = "Banjarbaru (BDJ)";
                        break;
                    case 3:
                        user[indexUser].passenger[indexPenumpang].ticket.airportFrom = "Bandar Udara Internasional Supadio"; // CEK WEBSITE
                        user[indexUser].passenger[indexPenumpang].ticket.IATA_from = "Pontianak (PNK)";
                        break;
                    case 4:
                        user[indexUser].passenger[indexPenumpang].ticket.airportFrom = "Bandar Udara Internasional Aji Pangeran Tumenggung Pranoto "; // CEK WEBSITE
                        user[indexUser].passenger[indexPenumpang].ticket.IATA_from = "Samarinda (AAP)";
                        break;
                    case 5:
                        user[indexUser].passenger[indexPenumpang].ticket.airportFrom = "Bandar Udara Internasional Juwata"; // CEK WEBSITE
                        user[indexUser].passenger[indexPenumpang].ticket.IATA_from = "Tarakan (TRK)";
                        break;
                    default:
                        cout<<"Wrong input, origin is undefined. Try again.";
                        lanjutReservasi = false;
                        getch();
                        break;
                }
                break;
            case 4:
                cout<<"\t\t Bandara Sulawesi\n";
                cout<<"1. Makassar (UPG)\n";
                cout<<"2. Manado (MDC)\n";
                cout<<">> "; cin>>inputReservation;
                switch (inputReservation){
                    case 1:
                        user[indexUser].passenger[indexPenumpang].ticket.airportFrom = "Bandar Udara Internasional Sultan Hassanuddin "; // CEK WEBSITE
                        user[indexUser].passenger[indexPenumpang].ticket.IATA_from = "Makassar (UPG)";
                    break;
                    case 2:
                        user[indexUser].passenger[indexPenumpang].ticket.airportFrom = "Bandar Udara Internasional Sam Ratulangi "; // CEK WEBSITE
                        user[indexUser].passenger[indexPenumpang].ticket.IATA_from = "Manado (MDC)";
                        break;
                    default:
                        break;
                } 
                break;
            case 5:
                cout<<"\t\t Bandara Nusa Tenggara\n";
                cout<<"1. Mataram (LOP)\n";
                cout<<">> "; cin>>inputReservation;
                switch (inputReservation){
                case 1:
                    user[indexUser].passenger[indexPenumpang].ticket.airportFrom = "Bandar Udara Internasional Lombok Zainuddin Abdul Madjid "; // CEK WEBSITE
                    user[indexUser].passenger[indexPenumpang].ticket.IATA_from = "Mataram (LOP)";
                    break;
                default:
                    cout<<"Wrong input, origin is undefined. Try again.";
                    lanjutReservasi = false;
                    getch();
                    break;
                }
                break;
            case 6:
                cout<<"\t\t Bandara Maluku\n";
                cout<<"1. Ambon (AMQ)\n";
                cout<<"2. Ternate (TTE)\n";
                cout<<">> "; cin>>inputReservation;
                switch (inputReservation){
                case 1:
                    user[indexUser].passenger[indexPenumpang].ticket.airportFrom = "Bandar Udara Pattimura "; // CEK WEBSITE
                    user[indexUser].passenger[indexPenumpang].ticket.IATA_from = "Ambon (AMQ)";
                    break;
                case 2:
                    user[indexUser].passenger[indexPenumpang].ticket.airportFrom = "Bandar Udara Sultan Babullah "; // CEK WEBSITE
                    user[indexUser].passenger[indexPenumpang].ticket.IATA_from = "Ternate (TTE)";
                    break;
                default:
                    cout<<"Wrong input, origin is undefined. Try again.";
                    lanjutReservasi = false;
                    getch();
                    break;
                }
                break;
            case 7:
                cout<<"\t\t Bandara Papua\n";
                cout<<"1. Biak (BIK)\n";
                cout<<"2. Jayapura (DJJ)\n";
                cout<<"3. Merauke (MKQ)\n";
                cout<<">> "; cin>>inputReservation;
                switch (inputReservation){
                case 1:
                    user[indexUser].passenger[indexPenumpang].ticket.airportFrom = "Bandar Udara Internasional Frans Kaisiepo "; // CEK WEBSITE
                    user[indexUser].passenger[indexPenumpang].ticket.IATA_from = "Biak (BIK)";
                    break;
                case 2:
                    user[indexUser].passenger[indexPenumpang].ticket.airportFrom = "Bandar Udara Internasional Sentani "; // CEK WEBSITE
                    user[indexUser].passenger[indexPenumpang].ticket.IATA_from = "Jayapura (DDJ)";
                    break;
                case 3:
                    user[indexUser].passenger[indexPenumpang].ticket.airportFrom = "Bandar Udara Internasional Mopah "; // CEK WEBSITE
                    user[indexUser].passenger[indexPenumpang].ticket.IATA_from = "Merauke (MKQ)";
                    break;
                default:
                    cout<<"Wrong input, origin is undefined. Try again.";
                    lanjutReservasi = false;
                    getch();
                    break;
                } 
                break;
            default:
                cout<<"Wrong input, origin is undefined. Try again.";
                lanjutReservasi = false;
                getch();
                break;
        }
    }while(!lanjutReservasi);
}

void arrival(){
    do{
        lanjutReservasi = true;
        system("cls");
        cout<<"\nDestination : "<<endl;
        cout<<"\t1. Sumatera\n\t2. Jawa dan Bali\n\t3. Kalimantan\n\t4. Sulawesi\n\t5. Nusa Tenggara\n\t6. Maluku\n\t7. Papua\n\t>> ";cin>>inputReservation;
        switch (inputReservation){
            case 1:
                cout<<"\t\t Bandara Sumatera\n";
                cout<<"1. Banda Aceh (BTJ)\n";
                cout<<"2. Lampung (TKG)\n";
                cout<<"3. Batam (BTH)\n";
                cout<<"4. Deli Serdang (KNO)\n";
                cout<<"5. Padang (PDG)\n";
                cout<<"6. Palembang (PLM)\n";
                cout<<"7. Pekanbaru (PKU)\n";
                cout<<"8. Bangka Belitung (TJQ)\n";
                cout<<"9. Tanjung Pinang (TNJ)\n";
                
                cout<<">> "; cin>>inputReservation;
                switch(inputReservation){
                    case 1:
                        user[indexUser].passenger[indexPenumpang].ticket.airportTo = "Bandar Udara Internasional Sultan Iskandar Muda "; // CEK WEBSITE
                        user[indexUser].passenger[indexPenumpang].ticket.IATA_to = "Banda Aceh (BTJ)";
                        break;
                    case 2:
                        user[indexUser].passenger[indexPenumpang].ticket.airportTo = "Bandar Udara Internasional Radin Inten II";
                        user[indexUser].passenger[indexPenumpang].ticket.IATA_to = "Bandar Lampung (TKG)";
                        break;
                    case 3:
                        user[indexUser].passenger[indexPenumpang].ticket.airportTo = "Bandar Udara Internasional Hang Nadim";
                        user[indexUser].passenger[indexPenumpang].ticket.IATA_to = "Batam (BTH)";
                        break;
                    case 4:
                        user[indexUser].passenger[indexPenumpang].ticket.airportTo = "Bandar Udara Internasional Kualanamu";
                        user[indexUser].passenger[indexPenumpang].ticket.IATA_to = "Deli Serdang (KNO)";
                        break;
                    case 5:
                        user[indexUser].passenger[indexPenumpang].ticket.airportTo = "Bandar Udara Internasional Minangkabau";
                        user[indexUser].passenger[indexPenumpang].ticket.IATA_to = "Padang (PDG)";
                        break;
                    case 6:
                        user[indexUser].passenger[indexPenumpang].ticket.airportTo = "Bandar Udara Internasional Sultan Mahmud Badarrudin";
                        user[indexUser].passenger[indexPenumpang].ticket.IATA_to = "Palembang (PLM)";
                        break;
                    case 7:
                        user[indexUser].passenger[indexPenumpang].ticket.airportTo = "Bandar Udara Internasional Sultan Syarif Kasim II";
                        user[indexUser].passenger[indexPenumpang].ticket.IATA_to = "Pekanbaru (PKU)";
                        break;
                    case 8:
                        user[indexUser].passenger[indexPenumpang].ticket.airportTo = "Bandar Udara Internasional H.A.S Hanandjoeddin";
                        user[indexUser].passenger[indexPenumpang].ticket.IATA_to = "Bangka Belitung (TJQ)";
                        break;
                    case 9:
                        user[indexUser].passenger[indexPenumpang].ticket.airportTo = "Bandar Udara Internasional Raja Haji Fisabillah";
                        user[indexUser].passenger[indexPenumpang].ticket.IATA_to = "Tanjung Pinang (TNJ)";
                        break;
                    default:
                        cout<<"Wrong input, destination is undefined. Try again.";
                        lanjutReservasi = false;
                        getch();
                        break;
                }
                break;
            case 2:
                cout<<"\t\t Bandara Jawa dan Bali\n";
                cout<<"1. Bandung (BDO)\n";
                cout<<"2. Bali (DPS)\n";
                cout<<"3. Jakarta (HLP)\n";
                cout<<"4. Tangerang (CGK)\n";
                cout<<"5. Majalengka (KJT)\n";
                cout<<"6. Semarang (SMG)\n";
                cout<<"7. Sidoarjo (SUB)\n";
                cout<<"8. Boyolali (SOC)\n";
                cout<<"9. Yogyakarta (JOG)\n";
                cout<<"10. Kulonprogo (YIA)\n";
                cout<<">> "; cin>>inputReservation;
                switch(inputReservation){
                case 1:
                    user[indexUser].passenger[indexPenumpang].ticket.airportTo = "Bandar Udara Internasional Husein Sastranegara "; // CEK WEBSITE
                    user[indexUser].passenger[indexPenumpang].ticket.IATA_to = "Bandung (BDO)";
                    break;
                case 2:
                    user[indexUser].passenger[indexPenumpang].ticket.airportTo = "Bandar Udara Internasional I Gusti Ngurah Rai "; // CEK WEBSITE
                    user[indexUser].passenger[indexPenumpang].ticket.IATA_to = "Denpasar (DPS)";
                    break;
                case 3:
                    user[indexUser].passenger[indexPenumpang].ticket.airportTo = "Bandar Udara Internasional Halim Perdanakusuma "; // CEK WEBSITE
                    user[indexUser].passenger[indexPenumpang].ticket.IATA_to = "Jakarta (HLP)";
                    break;
                case 4:
                    user[indexUser].passenger[indexPenumpang].ticket.airportTo = "Bandar Udara Internasional Soekarno-Hatta "; // CEK WEBSITE
                    user[indexUser].passenger[indexPenumpang].ticket.IATA_to = "Tangerang (CGK)";
                    break;
                case 5:
                    user[indexUser].passenger[indexPenumpang].ticket.airportTo = "Bandar Udara Internasional Kertajati"; // CEK WEBSITE
                    user[indexUser].passenger[indexPenumpang].ticket.IATA_to = "Majalengka (KJT)";
                    break;
                case 6:
                    user[indexUser].passenger[indexPenumpang].ticket.airportTo = "Bandar Udara Internasional Jenderal Ahmad Yani"; // CEK WEBSITE
                    user[indexUser].passenger[indexPenumpang].ticket.IATA_to = "Semarang (SMG)";
                    break;
                case 7:
                    user[indexUser].passenger[indexPenumpang].ticket.airportTo = "Bandar Udara Internasional Juanda"; // CEK WEBSITE
                    user[indexUser].passenger[indexPenumpang].ticket.IATA_to = "Sidoarjo (SUB)";
                    break;
                case 8:
                    user[indexUser].passenger[indexPenumpang].ticket.airportTo = "Bandar Udara Internasional Adisumarmo "; // CEK WEBSITE
                    user[indexUser].passenger[indexPenumpang].ticket.IATA_to = "Boyolali (SOC)";
                    break;
                case 9:
                    user[indexUser].passenger[indexPenumpang].ticket.airportTo = "Bandar Udara Internasional Adisucipto "; // CEK WEBSITE
                    user[indexUser].passenger[indexPenumpang].ticket.IATA_to = "Yogyakarta (JOG)";
                    break;
                case 10:
                    user[indexUser].passenger[indexPenumpang].ticket.airportTo = "Bandar Udara Internasional Yogyakarta"; // CEK WEBSITE
                    user[indexUser].passenger[indexPenumpang].ticket.IATA_to = "Kulonprogo (YIA)";
                    break;
                default:
                    cout<<"Wrong input, destination is undefined. Try again.";
                    lanjutReservasi = false;
                    getch();
                    break;
                }
                break;
            case 3: 
                cout<<"\t\t Bandara Kalimantan\n";
                cout<<"1. Balikpapan (BPN)\n";
                cout<<"2. Banjarbaru (BDJ)\n";
                cout<<"3. Pontianak (PNK)\n";
                cout<<"4. Samarinda (AAP)\n";
                cout<<"5. Tarakan (TRK)\n";
                cout<<">> "; cin>>inputReservation;
                switch (inputReservation){
                    case 1:
                        user[indexUser].passenger[indexPenumpang].ticket.airportTo = "Bandar Udara Internasional Sultan Aji Muhammad Sulaiman Sepinggan "; // CEK WEBSITE
                        user[indexUser].passenger[indexPenumpang].ticket.IATA_to = "Balikpapan (BPN)";
                        break;
                    case 2:
                        user[indexUser].passenger[indexPenumpang].ticket.airportTo = "Bandar Udara Internasional Syamsudin Noor "; // CEK WEBSITE
                        user[indexUser].passenger[indexPenumpang].ticket.IATA_to = "Banjarbaru (BDJ)";
                        break;
                    case 3:
                        user[indexUser].passenger[indexPenumpang].ticket.airportTo = "Bandar Udara Internasional Supadio"; // CEK WEBSITE
                        user[indexUser].passenger[indexPenumpang].ticket.IATA_to = "Pontianak (PNK)";
                        break;
                    case 4:
                        user[indexUser].passenger[indexPenumpang].ticket.airportTo = "Bandar Udara Internasional Aji Pangeran Tumenggung Pranoto "; // CEK WEBSITE
                        user[indexUser].passenger[indexPenumpang].ticket.IATA_to = "Samarinda (AAP)";
                        break;
                    case 5:
                        user[indexUser].passenger[indexPenumpang].ticket.airportTo = "Bandar Udara Internasional Juwata"; // CEK WEBSITE
                        user[indexUser].passenger[indexPenumpang].ticket.IATA_to = "Tarakan (TRK)";
                        break;
                    default:
                        cout<<"Wrong input, destination is undefined. Try again.";
                    lanjutReservasi = false;
                    getch();
                        break;
                }
                break;
            case 4:
                cout<<"\t\t Bandara Sulawesi\n";
                cout<<"1. Makassar (UPG)\n";
                cout<<"2. Manado (MDC)\n";
                cout<<">> "; cin>>inputReservation;
                switch (inputReservation){
                    case 1:
                        user[indexUser].passenger[indexPenumpang].ticket.airportTo = "Bandar Udara Internasional Sultan Hassanuddin "; // CEK WEBSITE
                        user[indexUser].passenger[indexPenumpang].ticket.IATA_to = "Makassar (UPG)";
                    break;
                    case 2:
                        user[indexUser].passenger[indexPenumpang].ticket.airportTo = "Bandar Udara Internasional Sam Ratulangi "; // CEK WEBSITE
                        user[indexUser].passenger[indexPenumpang].ticket.IATA_to = "Manado (MDC)";
                        break;
                    default:
                        break;
                } 
                break;
            case 5:
                cout<<"\t\t Bandara Nusa Tenggara\n";
                cout<<"1. Mataram (LOP)";
                cout<<">> "; cin>>inputReservation;
                switch (inputReservation){
                case 1:
                    user[indexUser].passenger[indexPenumpang].ticket.airportTo = "Bandar Udara Internasional Lombok Zainuddin Abdul Madjid "; // CEK WEBSITE
                    user[indexUser].passenger[indexPenumpang].ticket.IATA_to = "Mataram (LOP)";
                    break;
                default:
                    cout<<"Wrong input, destination is undefined. Try again.";
                    lanjutReservasi = false;
                    getch();
                    break;
                }
                break;
            case 6:
                cout<<"\t\t Bandara Maluku\n";
                cout<<"1. Ambon (AMQ)\n";
                cout<<"2. Ternate (TTE)\n";
                cout<<">> "; cin>>inputReservation;
                switch (inputReservation){
                case 1:
                    user[indexUser].passenger[indexPenumpang].ticket.airportTo = "Bandar Udara Pattimura "; // CEK WEBSITE
                    user[indexUser].passenger[indexPenumpang].ticket.IATA_to = "Ambon (AMQ)";
                    break;
                case 2:
                    user[indexUser].passenger[indexPenumpang].ticket.airportTo = "Bandar Udara Sultan Babullah "; // CEK WEBSITE
                    user[indexUser].passenger[indexPenumpang].ticket.IATA_to = "Ternate (TTE)";
                    break;
                default:
                    cout<<"Wrong input, destination is undefined. Try again.";
                    lanjutReservasi = false;
                    getch();
                    break;
                }
                break;
            case 7:
                cout<<"\t\t Bandara Papua\n";
                cout<<"1. Biak (BIK)\n";
                cout<<"2. Jayapura (DJJ)\n";
                cout<<"3. Merauke (MKQ)\n";
                cout<<">> "; cin>>inputReservation;
                switch (inputReservation){
                case 1:
                    user[indexUser].passenger[indexPenumpang].ticket.airportTo = "Bandar Udara Internasional Frans Kaisiepo "; // CEK WEBSITE
                    user[indexUser].passenger[indexPenumpang].ticket.IATA_to = "Biak (BIK)";
                    break;
                case 2:
                    user[indexUser].passenger[indexPenumpang].ticket.airportTo = "Bandar Udara Internasional Sentani "; // CEK WEBSITE
                    user[indexUser].passenger[indexPenumpang].ticket.IATA_to = "Jayapura (DDJ)";
                    break;
                case 3:
                    user[indexUser].passenger[indexPenumpang].ticket.airportTo = "Bandar Udara Internasional Mopah "; // CEK WEBSITE
                    user[indexUser].passenger[indexPenumpang].ticket.IATA_to = "Merauke (MKQ)";
                    break;
                default:
                    cout<<"Wrong input, destination is undefined. Try again.";
                    lanjutReservasi = false;
                    getch();
                    break;
                } 
                break;
            default:
                cout<<"Wrong input, destination is undefined. Try again.";
                lanjutReservasi = false;
                getch();
                break;
        }
    }while(!lanjutReservasi);
}

void date(){
    bool input;
    do{
        input = true;
        while(input){
            system("cls");
            cout << "Today : " << now_time->tm_mday << "/" << (now_time->tm_mon + 1) << "/"<< (now_time->tm_year + 1900) << endl;
            cout << "\t\t\tFlight Schedule\n";
            cout << "\t\t\t  dd/mm/yyyy" << endl << endl;
            user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.year = now_time->tm_year + 1901;

            cout << "Date : "; cin >> user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.date;
            if(user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.date > 31 || user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.date < 1){
                cout << "Invalid date. Try again.";
                input = false;
                lanjutReservasi = false;
                getch();
                break;
            }
            else break;
        }
        while(input){
            if(user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.date > 31 || user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.date < 1){
                break;
            }
            cout << "Month : "; cin >> user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.month;
                if(user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.month > 12 || user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.month < 1){
                    cout << "Invalid month. Try again.";
                    input = false;
                    lanjutReservasi = false;
                    getch();
                    break;
                }
                else break;
        }
        while(input){   
            if(user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.month > 12 || user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.month < 1) break;
            cout << "Year : "; cin >> user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.year;
            if(user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.year < now_time->tm_year + 1900){
                cout << "Invalid year, try again.";
                input = false;
                lanjutReservasi = false;
                getch();
            }
            else if(user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.date <= now_time->tm_mday && user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.month <= now_time->tm_mon + 1){
                if(user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.year <= now_time->tm_year+1900){
                    cout << "Invalid year, try again.";
                    input = false;
                    lanjutReservasi = false;
                    getch();
                }
                else break;
            }
            else if(user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.date >= now_time->tm_mday && user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.month < now_time->tm_mon + 1){
                if(user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.year <= now_time->tm_year+1900){
                    cout << "Invalid year, try again.";
                    input = false;
                    lanjutReservasi = false;
                    getch();
                }
                else break;
            }
            else break;
        }
        if(input){
            system("cls");
                cout << "\t\t\tFlight  Schedule\n";
                cout << "\t\t\t   ";
                if(user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.date < 10) cout << "0";
                            cout << user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.date << "/" ;
                if(user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.month < 10) cout << "0";
                            cout << user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.month << "/"
                                 << user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.year << endl << endl;
                cout << "\t\t\t Input Sukses!\n\n";

                do{
                    cout << "\nWant to change schedule ? (y/n)\n>> "; cin >> is_ulang;
                    if(is_ulang == 'y'){
                        lanjutReservasi = false;
                        break;
                    }
                    else if(is_ulang == 'n') lanjutReservasi = true;
                    else loopBack = true;
                }while(loopBack);
        }
    }while(!lanjutReservasi);

    do{
        input = true;
        system("cls");
        cout << "\t\t\tFlight  Schedule\n";
        cout << "\t\t\t   ";
        if(user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.date < 10) cout << "0";
            cout << user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.date << "/" ;
        if(user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.month < 10) cout << "0";
            cout << user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.month << "/"
                 << user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.year << endl << endl;
        cout << "\t\t\t     Time\n";
        cout << "\t\t\t     00:00" << endl << endl;

        while (input){
            cout << "Hour : "; cin >> user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.hour;
            if(user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.hour > 24 || user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.hour < 0){
                cout << "Invalid time. Try again.";
                input = false;
                lanjutReservasi = false;
                getch();
                break;
            }
            else break;
        }
        while(input){
            if(user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.hour > 24 || user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.hour < 0) break;
            cout << "Minute : "; cin >> user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.minute;
            if(user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.minute > 60 || user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.minute < 0){
                cout << "Invalid time. Try again.";
                input = false;
                lanjutReservasi = false;
                getch();
                break;
            }
            else break;
        }
        if(input){
            system("cls");
                cout << "\t\t\tFlight  Schedule\n";
                cout << "\t\t\t   ";
                if(user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.date < 10) cout << "0";
                    cout << user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.date << "/" ;
                if(user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.month < 10) cout << "0";
                    cout << user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.month << "/"
                    << user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.year << endl << endl;
                
                cout << "\t\t\t     Time\n";
                cout << "\t\t\t     ";
                if(user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.hour < 10) cout << "0";
                    cout << user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.hour << ":" ;
                if(user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.minute < 10) cout << "0";
                    cout << user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.minute << endl << endl;
                cout << "\t\t\t Input Sukses!\n\n";

                do{
                    cout << "\nWant to change schedule ? (y/n)\n>> ";cin >> is_ulang;
                    if(is_ulang == 'y'){
                        lanjutReservasi = false;
                        break;
                    }
                    else if(is_ulang == 'n') lanjutReservasi = true;
                    else loopBack = true;
                }while(loopBack);
        }
    }while(!lanjutReservasi);
}

void output(){
    string pay;
    bool invalid = false;
    system("cls");
    cout << "\t\t\t\tTicket Information\n\n";
    if(indexPenumpang == 0){
        cout << "You don't have ticket.\n\n";
        cout << "Go to main menu. . .  ";
    }
    else{
        for(int i = 0; i < indexPenumpang; i++){
        cout << "Ticket ID           : " << user[indexUser].passenger[i].ticket.ticket_id << endl;
        cout << "ID                  : " << user[indexUser].passenger[i].id << endl;
        cout << "Name                : " << user[indexUser].passenger[i].name <<  endl;
        cout << "Airline             : " << user[indexUser].passenger[i].ticket.airline << endl;
        cout << "FLight              : " << user[indexUser].passenger[i].ticket.flight << endl;
        cout << "Category            : " << user[indexUser].passenger[i].ticket.category << endl;
        cout << "Origin              : " << user[indexUser].passenger[i].ticket.IATA_from << endl;
        cout << "Departure           : " << user[indexUser].passenger[i].ticket.airportFrom << endl;
        cout << "Destination         : " << user[indexUser].passenger[i].ticket.IATA_to << endl;
        cout << "Arrival             : " << user[indexUser].passenger[i].ticket.airportTo << endl; 
        cout << "Date                : " ; if(user[indexUser].passenger[i].ticket.scheduleDate.date < 10) cout << "0";
                                        cout << user[indexUser].passenger[i].ticket.scheduleDate.date << "/" ;
                                    if(user[indexUser].passenger[i].ticket.scheduleDate.month < 10) cout << "0";
                                        cout << user[indexUser].passenger[i].ticket.scheduleDate.month << "/"
                                            << user[indexUser].passenger[i].ticket.scheduleDate.year << endl;
        cout << "Time                : " ;  if(user[indexUser].passenger[i].ticket.scheduleDate.hour < 10) cout << "0";
                                        cout << user[indexUser].passenger[i].ticket.scheduleDate.hour << ":" ;
                                    if(user[indexUser].passenger[i].ticket.scheduleDate.minute < 10) cout << "0";
                                        cout << user[indexUser].passenger[i].ticket.scheduleDate.minute << endl;   
        cout << "Price               : IDR. " << user[indexUser].passenger[i].ticket.price << endl;
        cout << "Payment Status      : " << user[indexUser].passenger[i].ticket.status << endl << endl;
        }  
    }
        for(int i= 0; i <= indexPenumpang; i++){
            if(user[indexUser].passenger[i].ticket.status == "Booked"){
                cout << "Klik \"pay\" to pay the booked ticket?\n>> ";
                cin >> pay;
                if(pay == "pay" || pay == "Pay"){
                    bayar();
                    break;
                }
                else{
                    cout << "\nBack to main menu.";
                    break;
                } 
            }
            else{
                if(i == indexPenumpang){
                    cout << "\n\nYou don't have ticket to pay. Back to main menu";
                    break;
                }
            } 
        }
    getch();
}

void bayar(){
    string delFileS;
    const char *delFile;
    long long nominal, paid = 0, hutang = 0, lebih = 0, sisa = 0, harga = 0;
    int ticket_id, indexBayar;
    bool invalid, bayar, ngutang = false;
    do{
        invalid = false;
        cout << "\n\nInput ticket (ID) which you want to pay\n>> ";
        cin >> ticket_id;
        for(int i= 0; i <= indexPenumpang; i++){
            if(ticket_id == user[indexUser].passenger[i].ticket.ticket_id){
                invalid = false;
                bayar = true;
                indexBayar = i;
                break;
            }
            else{
                if(i == indexPenumpang){
                    invalid = true;
                    bayar = false;
                    cout << "Ticket ID unavailable. Try again.";
                }
            }
        }
    }while(invalid);
    
    if(bayar){
        ifs.open("Database/Ticket/Ngutang/" + user[indexUser].passenger[indexBayar].name + " sisa.txt");
            if(ifs.is_open()){
                ifs >> sisa;
                ngutang = true;
            }
        ifs.close();

        system("cls");
        cout << "Ticket ID           : " << user[indexUser].passenger[indexBayar].ticket.ticket_id << endl;
        cout << "ID                  : " << user[indexUser].passenger[indexBayar].id << endl;
        cout << "Name                : " << user[indexUser].passenger[indexBayar].name <<  endl;
        cout << "Airline             : " << user[indexUser].passenger[indexBayar].ticket.airline << endl;
        cout << "FLight              : " << user[indexUser].passenger[indexBayar].ticket.flight << endl;
        cout << "Category            : " << user[indexUser].passenger[indexBayar].ticket.category << endl;
        cout << "Origin              : " << user[indexUser].passenger[indexBayar].ticket.IATA_from << endl;
        cout << "Departure           : " << user[indexUser].passenger[indexBayar].ticket.airportFrom << endl;
        cout << "Destination         : " << user[indexUser].passenger[indexBayar].ticket.IATA_to << endl;
        cout << "Arrival             : " << user[indexUser].passenger[indexBayar].ticket.airportTo << endl; 
        cout << "Date                : " ; if(user[indexUser].passenger[indexBayar].ticket.scheduleDate.date < 10) cout << "0";
                                        cout << user[indexUser].passenger[indexBayar].ticket.scheduleDate.date << "/" ;
                                    if(user[indexUser].passenger[indexBayar].ticket.scheduleDate.month < 10) cout << "0";
                                        cout << user[indexUser].passenger[indexBayar].ticket.scheduleDate.month << "/"
                                            << user[indexUser].passenger[indexBayar].ticket.scheduleDate.year << endl;
        cout << "Time                : " ;  if(user[indexUser].passenger[indexBayar].ticket.scheduleDate.hour < 10) cout << "0";
                                        cout << user[indexUser].passenger[indexBayar].ticket.scheduleDate.hour << ":" ;
                                    if(user[indexUser].passenger[indexBayar].ticket.scheduleDate.minute < 10) cout << "0";
                                        cout << user[indexUser].passenger[indexBayar].ticket.scheduleDate.minute << endl; 
        cout << "Price               : IDR. " << user[indexUser].passenger[indexBayar].ticket.price << endl;
    if(ngutang)
        cout << "Paid                : IDR. " << user[indexUser].passenger[indexBayar].ticket.price - sisa << endl;
        cout << "Payment Status      : " << user[indexUser].passenger[indexBayar].ticket.status << endl << endl;
    if(ngutang){
        cout << "You must pay " << sisa << " more.\n\n\n";
    }       

        cout << "Your Balance : " << user[indexUser].saldo << endl;
        cout << string(10 + to_string(user[indexUser].saldo).length() , '-');

        if(user[indexUser].saldo > 0){
            nominal = 0;
            do{
                if(nominal > user[indexUser].saldo) cout << "\nYour balance is not enough to pay.";
                if(nominal < 0) cout << "\nInvalid";

                cout << "\nPay Nominal     : ";
                cin  >> nominal;
            }while(nominal > user[indexUser].saldo || nominal < 0);
            bayar = true;
        }
        else{
            cout << "\n\nYou can't pay even 1 ticket. Top up first.";
            bayar = false;
        }
    }
    if(bayar){       
        cout << "Loading.";
        Sleep(1000);
        cout << ".";
        Sleep(1000);
        cout << ".";
        Sleep(1000);
        paid += nominal;

        if(ngutang){
            lebih = paid - sisa;

            if(paid == sisa){
                user[indexUser].passenger[indexBayar].ticket.status = "Paid";
                cout << "\nPayment success!";
                user[indexUser].saldo -= paid;
                delFileS = "Database/Ticket/Ngutang/" + user[indexUser].passenger[indexBayar].name + " sisa.txt";
                delFile = delFileS.c_str();
                int a = remove(delFile);
            }
            else if(paid > sisa){
                cout << "\nPayment success. You got cash back " << lebih;
                user[indexUser].passenger[indexBayar].ticket.status = "Paid";
                user[indexUser].saldo -= (paid - lebih);
                delFileS = "Database/Ticket/Ngutang/" + user[indexUser].passenger[indexBayar].name + " sisa.txt";
                delFile = delFileS.c_str();
                int a = remove(delFile);
            }
            else if(paid < sisa){
                user[indexUser].saldo -= paid;
                sisa -= paid;
                cout << "\nPayment success. Pay later " << sisa << " more.";
                ofs.open("Database/Ticket/Ngutang/" + user[indexUser].passenger[indexBayar].name + " sisa.txt");
                    ofs << sisa;    
                ofs.close();
            }
        }

        else{
            lebih = paid - user[indexUser].passenger[indexBayar].ticket.price;

            if(paid == user[indexUser].passenger[indexBayar].ticket.price){
                user[indexUser].passenger[indexBayar].ticket.status = "Paid";
                cout << "\nPayment success!";
                user[indexUser].saldo -= paid;
                delFileS = "Database/Ticket/Ngutang/" + user[indexUser].passenger[indexBayar].name + " sisa.txt";
                delFile = delFileS.c_str();
                int a = remove(delFile);
            }
            else if(paid > user[indexUser].passenger[indexBayar].ticket.price){
                cout << "\nPayment success. You got cash back " << lebih;
                user[indexUser].passenger[indexBayar].ticket.status = "Paid";
                user[indexUser].saldo -= (paid - lebih);
                delFileS = "Database/Ticket/Ngutang/" + user[indexUser].passenger[indexBayar].name + " sisa.txt";
                delFile = delFileS.c_str();
                int a = remove(delFile);
            }
            else if(paid < user[indexUser].passenger[indexBayar].ticket.price){
                user[indexUser].saldo -= paid;
                sisa = user[indexUser].passenger[indexBayar].ticket.price - paid;
                cout << "\nPayment success. Pay later " << sisa << " more.";
                ofs.open("Database/Ticket/Ngutang/" + user[indexUser].passenger[indexBayar].name + " sisa.txt");
                    ofs << sisa;    
                ofs.close();
            }
        }

        ofs.open("Database/Ticket/ string " + user[indexUser].username +  ".txt");
            for(int i = 0; i < indexPenumpang; i++){
                ofs << user[indexUser].passenger[i].name << endl;
                ofs << user[indexUser].passenger[i].ticket.airline << endl;
                ofs << user[indexUser].passenger[i].ticket.flight << endl;
                ofs << user[indexUser].passenger[i].ticket.category << endl;
                ofs << user[indexUser].passenger[i].ticket.IATA_from << endl;
                ofs << user[indexUser].passenger[i].ticket.airportFrom << endl;
                ofs << user[indexUser].passenger[i].ticket.IATA_to << endl;
                ofs << user[indexUser].passenger[i].ticket.airportTo << endl;
                ofs << user[indexUser].passenger[i].ticket.status << endl << endl;
            }
        ofs.close();
        ofs.open("Database/Ticket/ int " + user[indexUser].username +  ".txt");
            for(int i = 0; i < indexPenumpang; i++){
                ofs << user[indexUser].passenger[i].ticket.ticket_id << endl;
                ofs << user[indexUser].passenger[i].id << endl;
                ofs << user[indexUser].passenger[i].ticket.scheduleDate.date << endl;
                ofs << user[indexUser].passenger[i].ticket.scheduleDate.month << endl;
                ofs << user[indexUser].passenger[i].ticket.scheduleDate.year << endl;
                ofs << user[indexUser].passenger[i].ticket.scheduleDate.hour << endl;
                ofs << user[indexUser].passenger[i].ticket.scheduleDate.minute << endl;
                ofs << user[indexUser].passenger[i].ticket.price << endl << endl;
            }
        ofs.close();
        eksportAkunData(ofs); //eksport saldo
    }
}

void eksportAkunIndex(ofstream &ofs){ // Eksport index ke txt
	ofs.open("Akun/indexing.txt");
        ofs << userCount;
    ofs.close();
}

void importAkunIndex(ifstream &ifs){ //Import index ke txt
    ifs.open("Akun/indexing.txt",ios::in);
        ifs >> userCount;
    ifs.close();
}
void importAkunData(ifstream &ifs){
    string lanjut[1000];
    ifs.open("Akun/data.txt");
        for(int i = 0; i < userCount; i++) {
            ifs >> user[i].saldo;
            ifs.ignore();
            getline(ifs, user[i].username);
            getline(ifs, user[i].password);
            getline(ifs, lanjut[i]);
        }
    ifs.close();
}
void eksportAkunData(ofstream &ofs){
    ofs.open("Akun/data.txt");
        for(int i = 0; i < userCount; i++) {
            ofs << user[i].saldo << "\n"
                << user[i].username << endl
                << user[i].password << endl << endl;
        }
    ofs.close();
}
void eksportPassengerCount(ofstream &ofs){ //Import index ke txt
    ofs.open("Database/index penumpang/index " + user[indexUser].username + ".txt");
        ofs << indexPenumpang << endl;
    ofs.close();
}

void importPassengerCount(ifstream &ifs){ //Import index ke txt
    ifs.open("Database/index penumpang/index " + user[indexUser].username + ".txt");
        ifs >> user[indexUser].passengerCount;
    ifs.close();
}

void eksportPassengerTicket(ofstream &ofs){
    ofs.open("Database/Ticket/ string " + user[indexUser].username +  ".txt", ios::app);
        ofs << user[indexUser].passenger[indexPenumpang].name << endl;
        ofs << user[indexUser].passenger[indexPenumpang].ticket.airline << endl;
        ofs << user[indexUser].passenger[indexPenumpang].ticket.flight << endl;
        ofs << user[indexUser].passenger[indexPenumpang].ticket.category << endl;
        ofs << user[indexUser].passenger[indexPenumpang].ticket.IATA_from << endl;
        ofs << user[indexUser].passenger[indexPenumpang].ticket.airportFrom << endl;
        ofs << user[indexUser].passenger[indexPenumpang].ticket.IATA_to << endl;
        ofs << user[indexUser].passenger[indexPenumpang].ticket.airportTo << endl;
        ofs << user[indexUser].passenger[indexPenumpang].ticket.status << endl << endl;
    ofs.close();
    ofs.open("Database/Ticket/ int " + user[indexUser].username +  ".txt", ios::app);
        ofs << user[indexUser].passenger[indexPenumpang].ticket.ticket_id << endl;
        ofs << user[indexUser].passenger[indexPenumpang].id << endl;
        ofs << user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.date << endl;
        ofs << user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.month << endl;
        ofs << user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.year << endl;
        ofs << user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.hour << endl;
        ofs << user[indexUser].passenger[indexPenumpang].ticket.scheduleDate.minute << endl;
        ofs << user[indexUser].passenger[indexPenumpang].ticket.price << endl << endl;
    ofs.close();
}

void importPassengerTicket(ifstream &ifs){
    string lanjut[100];
    ifs.open("Database/Ticket/ string " + user[indexUser].username + ".txt");
        for(int i = 0; i < indexPenumpang; i++){
            getline(ifs, user[indexUser].passenger[i].name);
            getline(ifs, user[indexUser].passenger[i].ticket.airline);
            getline(ifs, user[indexUser].passenger[i].ticket.flight);
            getline(ifs, user[indexUser].passenger[i].ticket.category);
            getline(ifs, user[indexUser].passenger[i].ticket.IATA_from);
            getline(ifs, user[indexUser].passenger[i].ticket.airportFrom);
            getline(ifs, user[indexUser].passenger[i].ticket.IATA_to);
            getline(ifs, user[indexUser].passenger[i].ticket.airportTo);
            getline(ifs, user[indexUser].passenger[i].ticket.status);
            getline(ifs, lanjut[i]);
        }
    ifs.close();
    ifs.open("Database/Ticket/ int " + user[indexUser].username + ".txt");
        for(int i =0; i < indexPenumpang; i++){ 
            ifs >> user[indexUser].passenger[i].ticket.ticket_id;
            ifs >> user[indexUser].passenger[i].id;
            ifs >> user[indexUser].passenger[i].ticket.scheduleDate.date;
            ifs >> user[indexUser].passenger[i].ticket.scheduleDate.month;
            ifs >> user[indexUser].passenger[i].ticket.scheduleDate.year;
            ifs >> user[indexUser].passenger[i].ticket.scheduleDate.hour;
            ifs >> user[indexUser].passenger[i].ticket.scheduleDate.minute;
            ifs >> user[indexUser].passenger[i].ticket.price;
        }
    ifs.close();
}
