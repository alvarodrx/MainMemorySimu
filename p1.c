//
//	Programa para demostraci�n de memoria compartida.
//	Javier Abell�n, 10 Mayo 2002.
//

#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void main()
{
	key_t Clave;
	int Id_Memoria;
	int *Memoria = NULL;
	int i,j;

	//
	//	Conseguimos una clave para la memoria compartida. Todos los
	//	procesos que quieran compartir la memoria, deben obtener la misma
	//	clave. Esta se puede conseguir por medio de la funci�n ftok.
	//	A esta funci�n se le pasa un fichero cualquiera que exista y est�
	//	accesible (todos los procesos deben pasar el mismo fichero) y un
	//	entero cualquiera (todos los procesos el mismo entero).
	//
	Clave = ftok ("/bin/ls", 33);
	if (Clave == -1)
	{
		printf("No consigo clave para memoria compartida\n");
		exit(0);
	}

	//
	//	Creamos la memoria con la clave reci�n conseguida. Para ello llamamos
	//	a la funci�n shmget pas�ndole la clave, el tama�o de memoria que
	//	queremos reservar (100 enteros en nuestro caso) y unos flags.
	//	Los flags son  los permisos de lectura/escritura/ejecucion
	//	para propietario, grupo y otros (es el 777 en octal) y el
	//	flag IPC_CREAT para indicar que cree la memoria.
	//	La funci�n nos devuelve un identificador para la memoria reci�n
	//	creada.
	//
	Id_Memoria = shmget (Clave, sizeof(int)*100, 0777 | IPC_CREAT);
	if (Id_Memoria == -1)
	{
		printf("No consigo Id para memoria compartida\n");
		exit (0);
	}

	//
	//	Una vez creada la memoria, hacemos que uno de nuestros punteros
	//	apunte a la zona de memoria reci�n creada. Para ello llamamos a
	//	shmat, pas�ndole el identificador obtenido anteriormente y un
	//	par de par�metros extra�os, que con ceros vale.
	//
	Memoria = (int *)shmat (Id_Memoria, (char *)0, 0);
	if (Memoria == NULL)
	{
		printf("No consigo memoria compartida\n");
		exit (0);
	}

	//
	//	Ya podemos utilizar la memoria.
	//	Escribimos cosas en la memoria. Los n�meros de 1 a 10 esperando
	//	un segundo entre ellos. Estos datos ser�n los que lea el otro
	//	proceso.
	//
	for (i=0; i<10; i++)
	{
		for (j=0; j<100; j++)
		{
			Memoria[j] = i;
		}
		printf("Escrito %d\n", i);

		sleep (1);
	}

	//
	//	Terminada de usar la memoria compartida, la liberamos.
	//
	shmdt ((char *)Memoria);
	shmctl (Id_Memoria, IPC_RMID, (struct shmid_ds *)NULL);
}
