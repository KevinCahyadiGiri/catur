#include "tipe_bentukan.h"

void balik(address_list P, infotype_stack X, stack *termakan, int *poin_putih, int *poin_hitam, list *list_ada_putih, list *list_ada_hitam, queue *giliran, char c);

void undo(stack *history, stack *termakan, int *poin_putih, int *poin_hitam, list *list_ada_putih, list *list_ada_hitam, queue *giliran, int turn){
    infotype_stack X;
    address_list P;
    
    if(turn==0){ //belum ada gerakan
        printf("\nBelum ada gerakan.\n");
        printf("Undo gagal.\n");

    } else if(turn==1){ //baru putih yang gerak
        Pop(history, &X);
        P = First(*list_ada_putih);
        while((Info(P).posisiC != X.posisiC_baru) || (Info(P).posisiR != X.posisiR_baru))
            P = Next(P);
        //P sudah menunjuk kepada bidak yg ingin diundo
        Info(P).posisiC = X.posisiC_lama;
        Info(P).posisiR = X.posisiR_lama;
        Add(giliran, 1);

        printf("\nGerakan sebelumnya berhasil dibatalkan.\n");


    } else{ //game udh jalan biasa
        Pop(history, &X);                
        if (InfoTail(*giliran) == 1) { //giliran putih
            P = First(*list_ada_hitam);//hitam dulu yang diundo
            balik(P, X, termakan, poin_putih, poin_hitam, list_ada_putih, list_ada_hitam, giliran, 'h');
            Pop(history, &X);
            P = First(*list_ada_putih);//baru putih yang diundo
            balik(P, X, termakan, poin_putih, poin_hitam, list_ada_putih, list_ada_hitam,giliran, 'p');
        } else{ //giliran hitam
            P = First(*list_ada_putih);//putih dulu yang diundo
            balik(P, X, termakan, poin_putih, poin_hitam, list_ada_putih, list_ada_hitam,giliran, 'p');
            Pop(history, &X);
            P = First(*list_ada_hitam);//baru hitam yang diundo
            balik(P, X, termakan, poin_putih, poin_hitam, list_ada_putih, list_ada_hitam,giliran, 'h');
        }
        
        printf("\nGerakan sampai giliran sebelumnya berhasil dibatalkan.\n");
    }
}



void balik(address_list P, infotype_stack X, stack *termakan, int *poin_putih, int *poin_hitam, list *list_ada_putih, list *list_ada_hitam, queue *giliran, char c){
    infotype_stack Y;
    infotype_list Z;
    while((Info(P).posisiC != X.posisiC_baru) || (Info(P).posisiR != X.posisiR_baru)) {
        P = Next(P); }
    //P sudah menunjuk kepada bidak yg ingin diundo
    Info(P).posisiC = X.posisiC_lama;
    Info(P).posisiR = X.posisiR_lama;
    //cek kalau ada di stack termakan
    Y = (*termakan).T[(*termakan).TOP];

    //cek kalau abis en passant (makan pion lawan)
    if(X.enpassant){ 
        Pop(termakan,&Y);
        Z.nama = Y.nama;
        Z.player = Y.player;
        Z.poin = Y.poin;
        Z.posisiC = Y.posisiC_baru;
        Z.posisiR = Y.posisiR_baru;

        if(c=='h'){ //kalau hitam yang diundo dan putih yang dimakan
            InsVLast(list_ada_putih, Z);
            *poin_hitam -= Z.poin;
        } else{ //c=='p', kalau putih yang diundo dan hitam yang dimakan
            InsVLast(list_ada_hitam, Z);
            *poin_putih -= Z.poin;
        }
        
    } else if(X.turn == Y.turn){ //kalau barusan termakan
        Pop(termakan,&Y);
        Z.nama = Y.nama;
        Z.player = Y.player;
        Z.poin = Y.poin;
        Z.posisiC = Y.posisiC_baru;
        Z.posisiR = Y.posisiR_baru;

        if(c=='h'){ //kalau hitam yang diundo dan putih yang dimakan
            InsVLast(list_ada_putih, Z);
            *poin_hitam -= Z.poin;
        } else{ //c=='p', kalau putih yang diundo dan hitam yang dimakan
            InsVLast(list_ada_hitam, Z);
            *poin_putih -= Z.poin;
        }
    }


    //cek kalau abis promotion
    if(X.promotion){
        Info(P).nama = 'P';
        Info(P).poin = 1;
    }


    //cek kalau abis castling
    if(Info(P).nama == 'K'){
        if(abs(X.posisiC_baru-X.posisiC_lama) == 2){ //kalau raja gerak 2 kolom
            address_list P1, P2;
            boolean found = false;

            if (Info(P).player == 1) //giliran putih
                P1 = First(*list_ada_putih);
            else //giliran hitam
                P1 = First(*list_ada_hitam);

            P2 = P1;

            //cari R di mana
            while((P1!=Nil_list) && !found){
                if(Info(P1).nama == 'R'){
                    if((Info(P1).posisiR==Info(P).posisiR) && ((Info(P1).posisiC==4) || (Info(P1).posisiC==6))){
                        found = true;
                        P2 = P1;
                    }
                }
                P1 = Next(P1);
            }

            if(Info(P2).posisiC == 4) //kalau castling panjang
                Info(P2).posisiC = 1;
            else if(Info(P2).posisiC == 6) //kalau castling pendek
                Info(P2).posisiC = 8;
        }
    }
}