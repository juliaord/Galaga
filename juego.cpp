#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<stdlib.h>
#include<list>
using namespace std; ///Para que funcione nuestro #include<list>

#define ARRIBA 72       
#define IZQUIERDA 75
#define DERECHA 77
#define ABAJO 80

void gotoxy (int x, int y){ ///Estos parametros le pasan valores de X y Y a los atributos de COORD
    HANDLE hCon; ///Identificador de windows
    hCon = GetStdHandle (STD_OUTPUT_HANDLE);   ///Tomar el control de salida en la consola 
    COORD dwPos; ///Coordenadas en X y Y
    dwPos.X = x;
    dwPos.Y = y;

    SetConsoleCursorPosition(hCon, dwPos); ///Función para darle una posición al cursor

}

void OcultarCursor () {
    HANDLE hCon;
    hCon = GetStdHandle (STD_OUTPUT_HANDLE);  ///Ocultar el cursor de la pantalla, para que no esté titilando y molestando al usuario 
    CONSOLE_CURSOR_INFO nnm; ///Controlar como se ve el cursor en pantalla
    nnm.dwSize = 2;
    nnm.bVisible = FALSE;

    SetConsoleCursorInfo(hCon,&nnm);

}

void dibujar_limites (){
    for (int i=2 ; i<78; i++){                
        gotoxy(i, 3); printf("%c",205); ///Imprime los limites superiores e inferiores 
        gotoxy(i, 33); printf("%c",205);
    }

    for (int i = 4 ; i < 33; i++) {
        gotoxy(2,i); printf("%c",186);  ///Imprime los limites laterales 
        gotoxy(77, i); printf ("%c",186);
    }

    gotoxy(2,3);printf("%c",201);
    gotoxy(2,33);printf("%c",200);
    gotoxy(77,3);printf("%c",187);  ///Imprime las esquinas de los limites 
    gotoxy(77,33);printf("%c",188);
}

class NAVE {
    int x,y;
    int corazones;
    int vidas;
public: 
   NAVE (int _x , int _y, int _corazones, int _vidas): x(_x), y(_y), corazones(_corazones), vidas(_vidas){}  
   int X(){ return x; } ///Las coordenadas x, y son privadas, por lo que tomamos una 
   int Y(){ return y; }
   int VIDS(){ return vidas; } ///X mayúscula y le retornamos el valor x para que otros métodos de otras clases puedan acceder a ellas
   void CORA() { corazones--; }
   void dibujar (); ///Métodos de la clase nave
   void borrar ();
   void mover ();
   void dibujar_corazones ();
   void morir ();

};


void NAVE::dibujar() {
     gotoxy (x,y);   printf ("   %c", 30);  ///Dibujar la forma de la nave con caracteres ASCII         
     gotoxy (x,y+1); printf ("  %c%c%c",40, 207, 41); 
     gotoxy (x,y+2); printf (" %c%c %c%c",30,190,190,30);
}

void NAVE::borrar (){
    gotoxy (x,y);   printf ("       ");  
    gotoxy (x,y+1); printf ("       ");  ///Borrará la huella/rastro que deje la nave en pantalla
    gotoxy (x,y+2); printf ("       ");

}

void NAVE::mover (){
    if (kbhit() ){ ///Función que detecta si se presiona una tecla
            char tecla = getch(); ///Pasarle el valor de la tecla presionada a la variable tecla
            borrar ();
            if(tecla == IZQUIERDA && x > 3) x--; ///Mover hacía la izquierda y no permitir que la nave sobrepase los limites
    		if(tecla == DERECHA && x+6 < 76) x++; ///Mover hacía la derecha y no permitir que la nave sobrepase los limites
    		if(tecla == ARRIBA && y > 4) y--; ///Subir y no permitir que la nave sobrepase los limites
    		if(tecla == ABAJO && y+3 < 33) y++; ///Bajar y no permitir que la nave sobrepase los limites
    		dibujar();
    		if(tecla == 'e')corazones--;
    		dibujar();
    		dibujar_corazones();
    		
    }
}

void NAVE::dibujar_corazones(){
	
	gotoxy(50,2); printf("Vidas %d",vidas);
	gotoxy(64,2); printf("Salud");
	gotoxy(70,2); printf("     ");  ///Imprime la cantidad de corazones que tiene el usuario
	for(int i = 0 ; i < corazones; i++){
		gotoxy(70+i,2); printf("%c",3);
	}
}

void NAVE::morir(){
	if(corazones == 0){
		borrar();
		gotoxy(x,y);   printf("   **   "); ///Explosión de la nave cuando la barra de corazones llega a 0
	    gotoxy(x,y+1); printf("  ****  ");
	    gotoxy(x,y+2); printf("   **   ");
	    Sleep(200);
	    
	    borrar();
	    gotoxy(x,y);   printf(" * ** *");
	    gotoxy(x,y+1); printf("  **** ");
	    gotoxy(x,y+2); printf(" * ** *");
	    Sleep (200);
	    
	    borrar();
	    vidas--;
	    corazones = 3;
	    dibujar_corazones();
	    dibujar();
    }
}

class ASTE{  ///Creación de la clase asteroides 
	int x,y;
public:
	ASTE(int _x, int _y): x(_x),y(_y){}
	void dibujar();
	void mover();
	void golpe(class NAVE &J);
	int X() { return x; }
    int Y() { return y; }
	
};

void ASTE::dibujar(){
	gotoxy(x,y); printf("%c",254); ///Carácter ASCII que será el asteroide
}

void ASTE::mover(){
	gotoxy(x,y); printf(" ");
	y++;
	if(y > 32){
		x = rand() % 71 + 4;
		y = 4;   ///Le asignaremos posiciones al azar dentro de la consola dados los límites que tiene la nave
	}
	dibujar();
}

void ASTE::golpe(class NAVE & J){
	if(x >= J.X() && x < J.X()+6 && y >= J.Y() && y <= J.Y()+2) ///Intervalo que ocupa la nave desde x hasta x+5 (ancho) 
	                                                            ///Y de y hasta y+2 (largo)
	{
		J.CORA();
		J.borrar();
		J.dibujar();
		J.dibujar_corazones();
		x = rand() % 71 + 4;
		y = 4; 
	}
}

class DISPARO{ ///Creación de la clase DISPARO para generar balas 
	int x,y;
public: 
    DISPARO(int _x, int _y): x(_x), y(_y){}
    int X() { return x; }
    int Y() { return y; }
    void mover();
    bool limite();
};

void DISPARO::mover() {
	gotoxy(x,y); printf(" ");
	y--;
	gotoxy(x,y); printf("*");
}

bool DISPARO::limite() {
	if(y == 4) return true;
	return false;
}

int main () {
 
    OcultarCursor ();
    dibujar_limites();
    NAVE N (37,30,3,3);
    N.dibujar ();
    N.dibujar_corazones();
    
    list<ASTE*> A;
    list <ASTE*>::iterator ttA; 
    for (int i=0; i<9; i++) {
    	A.push_back(new ASTE(rand() %75 + 3, rand () %5 + 4));
	}
    
    list<DISPARO*> D; ///Pasarle punteros
    list<DISPARO*>::iterator tt;

    bool game_over = false;
    int puntos = 0;
    while (!game_over){  ///Ejecutar el juego mientras game_over sea falso
    
            gotoxy (4,2); printf("Puntos %d", puntos);
            
			if(kbhit ())
            {
            	
            	char tecla = getch();
            	if (tecla == 'w')
            	D.push_back(new DISPARO(N.X() + 2, N.Y() - 1));
			}
			
			for(tt = D.begin(); tt != D.end(); tt++)
			{
				(*tt) -> mover();
				if((*tt) -> limite()){
					gotoxy((*tt) -> X(), (*tt) -> Y()); printf(" ");
					delete(*tt);
					tt = D.erase(tt); ///Indicarle al iterador que continue con la siguiente posición a pesar de haber borrado a su anterior en la lista 
				}
			}
            
            for(ttA = A.begin(); ttA != A.end(); ttA++){
            	(*ttA) -> mover();
            	(*ttA) -> golpe(N);
			}
			
			for(ttA = A.begin(); ttA != A.end(); ttA++){
				for(tt = D.begin(); tt != D.end(); tt++) {
					if((*ttA) -> X() == (*tt) -> X()  && (*ttA) -> Y() == (*tt) -> Y()  || (*ttA) -> Y() == (*tt) -> Y()){
						
						gotoxy((*tt) -> X() , (*tt) -> Y()); printf(" ");
						delete (*tt);
						tt = D.erase(tt);
						
						A.push_back(new ASTE(rand() %74 +3, 4));
						gotoxy((*ttA) -> X() , (*ttA) -> Y()); printf(" ");
						delete(*ttA);
						ttA = A.erase(ttA);
						
						puntos+=7;
					}
					
				}
				
			}
            if (N.VIDS() == 0) game_over = true;
			N.morir(); 
            N.mover ();
            Sleep (30);   ///Pausa para que el cursor no titile tanto

}
    return 0;

}
