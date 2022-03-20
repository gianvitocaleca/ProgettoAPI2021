//Graph Ranker - Gianvito Caleca

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct maxheap{
        int indice;
        unsigned long int s_dist;
};

/* PROTOTIPI FUNZIONI */
void MaxHeapify(struct maxheap *v, int n, int heapsize);
void CancellaMax(struct maxheap *v, int heapsize);
void Inserisci(struct maxheap *v,int ind,unsigned long int dist,int heapsize);
unsigned long int Dijkstra(unsigned long int **g,int dim);
void AggiungiGrafo(unsigned long int **g,int d,int kem,int graph_index,struct maxheap *c,int inseriti);
void TopK(struct maxheap *c, int inseriti);

/* MAIN */
int main(){
    int d=0,k=0,inseriti=0;
    int i=0,j=0;
    char comando[7];
    int graph_index=0;
    int moltiplicatore=1;
    char letto;
    unsigned long int **grafo;
    struct maxheap *classifica;
        //lettura di D
            do{
                comando[i]=getchar_unlocked();
                i++;
            }while(comando[i-1]!=' ');

            moltiplicatore=1;

            for(i=i-2;i>=0;i--){
                d=d+((comando[i]-'0')*moltiplicatore);
                moltiplicatore=moltiplicatore*10;
            }
        //lettura di K
            i=0;
            do{
                comando[i]=getchar_unlocked();
                i++;
            }while(comando[i-1]!='\n');

            moltiplicatore=1;

            for(i=i-2;i>=0;i--){
                k=k+((comando[i]-'0')*moltiplicatore);
                moltiplicatore=moltiplicatore*10;
            }

        //dopo la lettura di D e K, viene dimensionata la matrice di adiacenza del grafo e la classifica dei topk
        grafo=(unsigned long int **)malloc(d* sizeof(unsigned long int*)); //alloca righe
        for(i=0;i<d;i++){   //alloca colonne
            grafo[i]=(unsigned long int *)malloc(d* sizeof(unsigned long int*));
        }
        
        for(i=0;i<d;i++){   //inizializza il grafo
            for(j=0;j<d;j++){
                grafo[i][j]=0;
            }
        }

        classifica=(struct maxheap*)malloc(k* sizeof(struct maxheap));  //alloca classifica

        for(i=0;i<k;i++){   //inizializza classifica
            classifica[i].indice=0;
            classifica[i].s_dist=4294967295;    
        }

        //Lettura dei comandi
        do{
            letto=getchar_unlocked();
            if(letto=='A'){
                for(i=0;i<13;i++){
                    getchar_unlocked();
                }

                if(inseriti<k){
                        inseriti++;
                }

                AggiungiGrafo(grafo,d,k,graph_index,classifica,inseriti);
                graph_index=graph_index+1;
            }
            else if(letto!=EOF){
                for(i=0;i<4;i++){
                    getchar_unlocked();
                }
                    TopK(classifica,inseriti);
            }

        }while(letto!=EOF);

        //libera il povero heap
        for(i=0;i<d;i++){
            free(grafo[i]);
        }
        free(grafo);
        free(classifica);
    return 0;
}

/* FUNZIONI */

void MaxHeapify(struct maxheap *v, int n, int heapsize){
    int l=0,r=0;
    int posmax=0;
    struct maxheap p;

    l=2*n+1;
    r=l+1;

    if(l<=heapsize && v[l].s_dist>v[n].s_dist){
        posmax=l;
    }
    else{
        posmax=n;
    }

    if(r<=heapsize && v[r].s_dist>v[posmax].s_dist){
        posmax=r;
    }

    if(posmax!=n){
        p=v[n];
        v[n]=v[posmax];
        v[posmax]=p;
        MaxHeapify(v,posmax,heapsize);   
    }
    return;
}

void CancellaMax(struct maxheap *v, int heapsize){
    v[0]=v[heapsize];
    MaxHeapify(v,0,heapsize-1);
    return;
}

void Inserisci(struct maxheap *v,int ind,unsigned long int dist,int heapsize){   
    int parent;
    int i=0;
    struct maxheap p;

    v[heapsize].indice=ind;
    v[heapsize].s_dist=dist;
    i=heapsize;
    
    //trovaparent
    if(i%2==0){
        parent=(i/2)-1;
    }
    else{
        parent=i/2;
    }
    while(i>0 && v[parent].s_dist<v[i].s_dist){
        p=v[parent];
        v[parent]=v[i];
        v[i]=p;
        i=parent;

        if(i%2==0){
            parent=(i/2)-1;
        }
        else{
            parent=i/2;
        }
    }
    return;
} 

unsigned long int Dijkstra(unsigned long int **g,int dim){
    int i=0,j=0;
    int stop=1;
    int minindex=0;

    unsigned long int minimo;
    minimo=999999999999999999;    
    unsigned long int somma=0;
    unsigned long int ndist=0;

    unsigned long int distanze[dim]; //vettore delle distanze
    bool t[dim]; //vettore dei nodi senza distanza minima calcolata

    //Inizializzo con 0 ed i suoi vicini:
    distanze[0]=0;

    for(i=1;i<dim;i++){
        if(g[0][i]!=0){
            distanze[i]=g[0][i];
        }
        else{
            distanze[i]=999999999999999999;
        }
    }

    //Inizializzo il vettore dei nodi senza distanza minima calcolata
    for(i=0;i<dim;i++){
        t[i]=0;
    }

    for(j=1;j<dim;j++){ //chi è raggiungibile da 0, ha bisogno che la sua distanza minima venga aggiornata
        for(i=0;i<dim;i++){
            if(g[i][j]!=0){
                t[j]=1;
            }
        }
    }    

    do{
        //se per ogni i in T la distanza è infinito (nessun nodo deve essere aggiornato) fermati
        for(i=0;i<dim;i++){
            if(t[i]==1 && distanze[i]!=999999999999999999){
                stop=0;
            }
        }

        //se non devo fermarmi
        if(stop!=1){
            //trovo l'elemento con distanza minima in T
            minimo=999999999999999999;
            for(i=0;i<dim;i++){
                if(t[i]==1 && distanze[i]<=minimo){
                    minimo=distanze[i];
                    minindex=i; //mi segno il suo indice
                }
            }

            t[minindex]=0;  //il minimo non deve più aggiornare la distanza

            //verifica di aver aggiornato tutte le distanze
            stop=1;
            for(i=0;i<dim;i++){
                if(t[i]==1){
                    stop=0; //se c'è ancora qualcuno da aggiornare, non mi fermo
                }
            }
        }

        if(stop!=1){ //se devo ancora aggiornare distanze         
            for(i=0;i<dim;i++){ //aggiorna le distanze
                ndist=distanze[minindex]+g[minindex][i];
                if(t[i]==1 && g[minindex][i]!=0 && distanze[i]>ndist){    
                    distanze[i]=ndist;
                }
            }
        }
    }while(stop!=1);

    for(i=0;i<dim;i++){ 
        if(distanze[i]!=999999999999999999){
            somma=somma+distanze[i];  //somma tutte le distanze
        }
    }    
        return somma;
} 

void AggiungiGrafo(unsigned long int **g,int d,int kem,int graph_index,struct maxheap *c,int inseriti){
    int i=0,j=0;
    int letto=0;
    unsigned long int som=0;
    int stop=0;

    for(i=0;i<d;i++){ //lettura del grafo
        for(j=0;j<d;j++){
            stop=0;
            g[i][j]=0;
            do{
                letto=getchar_unlocked();
                if(letto==','||letto=='\n'){
                    stop=1;
                }
                else{
                    letto=letto-'0';
                    g[i][j]=(g[i][j]*10)+letto;
                }
            }while(stop!=1);
        }
    }
    som=Dijkstra(g,d);

    //Gestione della classifica
        if(inseriti<kem){   //inserisci in classifica i primi k elementi
            Inserisci(c,graph_index,som,inseriti-1);
        }
        else{   //dal k+1-esimo elemento in poi
            if(c[0].s_dist>som){    //se il massimo della classifica ha somma maggiore a quella del grafo corrente, inserisco il grafo in classifica
                CancellaMax(c,kem-1);
                Inserisci(c,graph_index,som,kem-1);
            }
        }
    return;
}

void TopK(struct maxheap *c, int inseriti){
    int i=0;
    if(inseriti==0){
        printf("\n");
    }
    else{
        for(i=0;i<inseriti-1;i++){
            printf("%d ",c[i].indice);
        }   
        printf("%d\n",c[i].indice);        
    }                               
    return;
}