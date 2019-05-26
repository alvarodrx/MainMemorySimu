#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

pid_t pid;
key_t clave;	//Clave de acceso a la zona de memoria
long int id;	//Identificador de la zona de memoria
int *pmem=NULL;	//Puntero a la zona de memoria

int main(int argc, char *argv[]) {

	//Creamos un área de memoria compartida
	clave=ftok("/bin/ls",33); //Cualquier fichero existente y cualquier int
	id=shmget(clave,sizeof(int)*100,0777|IPC_CREAT);
	pmem=(int *)shmat(id,(char *)0,0);

	pid = fork();
	if (pid == 0){
		//Soy el proceso hijo
		pmem[5] = 17; //Escribo en la memoria
	}
	else{
		//Soy el proceso padre
		usleep(100);
		printf("Leído: %d", pmem[5]);
		shmdt((char *)id); //Desconecta el segmento de memoria compartida
		shmctl(id,IPC_RMID,0); //Elimina el segmento de memoria compartida
	}
}
