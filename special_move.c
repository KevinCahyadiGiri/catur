#include "tipe_bentukan.h"
#include "castling.c"

boolean cek_enpassant(stack* history, list *list_ada_putih, list *list_ada_hitam, queue *giliran, address_list *P);
void enpassant(stack *history, stack *termakan, int *poin_putih, int *poin_hitam, list *list_ada_putih, list *list_ada_hitam, queue *giliran, int turn, address_list P);

void special_move(stack *history, stack *termakan, int *poin_putih, int *poin_hitam, list *list_ada_putih, list *list_ada_hitam, queue *giliran, int turn){
    address_list P;
    /*
    if(!cek_enpassant() && !cek_castling()){ //ga bisa dua-duanya
        printf("\nTidak ada gerakan khusus yang bisa dilakukan.\n");

    } else if(cek_enpassant() && cek_castling()){ //bisa dua-duanya
        printf("\nDaftar gerakan khusus yang bisa dilakukan:\n");
        printf("   1. Castling\n");
        printf("   2. En Passant\n");
        printf("Pilih gerakan khusus yang ingin dilakukan: ");
        //kalau 1
            castling();
            printf("Castling berhasil dilakukan\n");
        //kalau 2
            enpassant();
            printf("En Passant berhasil dilakukan\n");
    
    }*/ 
    if(cek_enpassant(history, list_ada_putih, list_ada_hitam, giliran, &P)){
        enpassant(history, termakan, poin_putih, poin_hitam, list_ada_putih, list_ada_hitam, giliran, turn, P);
        printf("En Passant berhasil dilakukan\n");
    } else{
        printf("\nTidak ada gerakan khusus yang bisa dilakukan.\n");
    }

    if(cek_castling_pendek(*history, *list_ada_putih, *list_ada_hitam, giliran)) // masih salah, ga kompatibel yg gw gapake pointer - palkon
    {
        printf("2. Castling Pendek");
    }

    if(cek_castling_panjang(*history, *list_ada_putih, *list_ada_hitam, giliran))
    {
        printf("3. Castling Panjang");
    }

    int input;
    scanf("%d", &input);

    if(input == 2)
    {
        castling_pendek(history, list_ada_putih, list_ada_hitam, giliran);
    }

    else if(input == 3)
    {
        castling_panjang(history, list_ada_putih, list_ada_hitam, giliran);
    }
}

boolean cek_enpassant(stack* history, list *list_ada_putih, list *list_ada_hitam, queue *giliran, address_list *P){
    boolean found=false;
    if ((*history).T[(*history).TOP].twosteps){ //pion musuh baru jalan 2 kotak
        //dicek di sebelah pion musuh tsb ada pion temen atau tidak
        if (InfoTail(*giliran) == 1) //giliran putih
            *P = First(*list_ada_putih);
        else //giliran == 2, hitam
            *P = First(*list_ada_hitam);

        while((*P) != Nil_list){
            if(Info(*P).nama == 'P'){ //kalau pion
                if(Info(*P).posisiR == (*history).T[(*history).TOP].posisiR_baru){ //kalau satu baris
                    if(abs(Info(*P).posisiC - (*history).T[(*history).TOP].posisiC_baru) == 1) //kalau sebelahan
                        found=true; //pion temen sebelahan dengan pion musuh tsb
                        break;
                }
            }
            *P = Next(*P);
        }
    }
    return found;
}

void enpassant(stack *history, stack *termakan, int *poin_putih, int *poin_hitam, list *list_ada_putih, list *list_ada_hitam, queue *giliran, int turn, address_list P){
    infotype_stack X;
    address_list A, A1;
    int tempR = Info(P).posisiR;
    int tempC = Info(P).posisiC;

    Info(P).posisiC = (*history).T[(*history).TOP].posisiC_baru;

    if (InfoTail(*giliran) == 1){ //giliran putih
        Info(P).posisiR--;
        *poin_putih = *poin_putih + 1;

        A1 = Search(*list_ada_hitam, Info(P).posisiC, tempR); //address sebelum bidak lawan yang termakan
        A = Next(A1); //address bidak lawan yang termakan
        DelAfter(list_ada_hitam, &A, A1); // hapus bidak lawan dari list linier lawan 

    } else{ //giliran == 2, hitam
        Info(P).posisiR++;
        *poin_hitam = *poin_hitam + 1;
        
        A1 = Search(*list_ada_putih, Info(P).posisiC, tempR); //address sebelum bidak lawan yang termakan
        A = Next(A1); //address bidak lawan yang termakan
        DelAfter(list_ada_putih, &A, A1); // hapus bidak lawan dari list linier lawan 
    }

    //bidak lawan yg termakan
    X.nama = Info(P).nama; 
    X.player = (InfoTail(*giliran) % 2) + 1; //lawan
    X.poin = 1;
    X.turn = turn;
    X.posisiR_lama = X.posisiR_baru = InfoTail(*giliran)==1 ? ++Info(P).posisiR : --Info(P).posisiR;
    X.posisiC_lama = X.posisiC_baru = Info(P).posisiC;
    X.enpassant = true;
    Push(termakan, X); //masukkan ke stack termakan

    //bidak yg barusan makan en passant
    X.nama = Info(P).nama; 
    X.player = InfoTail(*giliran);
    X.posisiR_lama = tempR;
    X.posisiC_lama = tempC;
    X.posisiR_baru = InfoTail(*giliran)==1 ? --tempR : ++tempR;
    Push(history, X); //masukkan ke stack history

    // update nilai awal di list_ada menjadi nilai akhir
    Info(P).posisiR = X.posisiR_baru;

    if (InfoTail(*giliran) == 1) //giliran berubah
        Add(giliran, 2);
    else
        Add(giliran, 1);
}



boolean HasMoved(piece P, stack history) //ngecek apakah raja ada di stack history ato ngga
{ //note for some reason pas gw cek indeks 1 ama 2 ga kedeteksi. semoga kompi gw doang.
    infotype_stack X;
    int counter = 1;
    boolean Moved = false;
    int jml = history.TOP;
    while ((!Moved) && (counter <= jml)){   
        X = (history).T[counter];
        counter++;
        if (X.nama == P.nama)
            Moved = true;
    }
    return Moved;
}

void castling(piece Raja, piece Benteng, stack history, papan* board[10][10])
{
    if (!(HasMoved(Raja, history))) //kalau raja belum pernah gerak
    {
        if (((*board[Raja.posisiR][Raja.posisiC+2]).nama == ' ') && ((*board[Raja.posisiR][Raja.posisiC+1]).nama == ' ') && (!(HasMoved(Benteng, history))))
        {
            // (*board[Raja.posisiR][Raja.posisiC+2]).nama == 'R';
        }
    }
}