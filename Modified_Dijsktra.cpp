#include <iostream>
#include <list>

using namespace std;

int NodoInicio,NodoFinal;//Variables globales de posicion

class NodosAdyacentes//Clase que representa las esquinas adyacentes a una esquina
{
	public:
		
		//Atributos
		int Nombre;//Identificador
		int Peso;//Peso de la arista
		int Choque;//Tiempo en el que ocurre el choque
		
		//Metodos
		NodosAdyacentes();//Constructor sin parametros
		NodosAdyacentes(int Nombre,int Peso,int C);//Constructor con parametros

};
//Definicion de metodos de la clase NodosAdyacentes
NodosAdyacentes::NodosAdyacentes()
{
	Nombre=-1;
	Peso=0;
	Choque=-1;
}

NodosAdyacentes::NodosAdyacentes(int N,int P,int C)
{
	Nombre=N;
	Peso=P;
	Choque=C;
}


class Nodo//Clase que representa las esquinas
{
	public:
		//Atributos
		int Nombre;//Identificador de la esquina
		int DuracionSemaforo;//Tiempo de duracion del semaforo [0,k]
		list<NodosAdyacentes> Adyacentes;//Lista de Esquinas adyacentes a la esquina actual
		int Costo;//Costo minimo acumulado en un determinado instante de tiempo
		bool Visitado;//Indica si ya se visito dicha esquina

		//Metodos
		Nodo();//Constructor sin parametros
		Nodo(int N,int DS,int C);//Constructor con parametros
};
//Definicion de los metodos de la clase Nodo
Nodo::Nodo()
	{
			int Nombre=-1;
			DuracionSemaforo=0;
			Adyacentes=list<NodosAdyacentes>();
			Costo=-1;
			Visitado=false;
	}	

Nodo::Nodo(int N,int DS,int C)
{
	Nombre=N;
	DuracionSemaforo=DS;
	Costo=C;
}

class Grafo//Clase Madre del algoritmo, desde la cual se cargan los grafos y se procesan dichos datos para emitir un resultado
{
	public:

		//Atributos
		int NumNodos;//Numero de esquinas existentes
		list<Nodo> Esquinas;//Lista con la informacion de las esquinas

		//Metodos
		Grafo();//Constructor con sin parametros para inicializar atributos de la clase
		void AgregarGrafo(int NNodos,int NAristas);//Carga los datos de un grafo dado
		bool SemaforoVerde(int TiempoAcumulado,int DurSemaforo);//Retorna verdad||falso dependiendo si el semaforo de una esquina esta en rojo o verde en un determinado momento
		int DiferenciaSemaforoRojo(int TiempoAcumulado,int DurSemaforo);//Retorna el costo actual + el costo necesario para poder ingresar a una esquina en caso de que un semaforo este en rojo
		int Minimo(list<Nodo> A);//Recorre la lista de esquinas del grafo y retorna el identificador de la esquina no visitada con menor costo en un momento
		void Dijkstra(Grafo Graf,int Inicio,int Final);//Retorna el costo minimo para ir de un lugar inicio a un lugar destino 
};
//Implementacion de los metodos de la clase Grafo
Grafo::Grafo()
		{
			NumNodos=0;
			Esquinas=list<Nodo>();
		}

void Grafo::AgregarGrafo(int NNodos,int NAristas)
{
	int Inicio=0,Final=0,Valor=0,Choque=0,DurSemaforo=0;//Inicializo las variables a leer en 0

	NumNodos=NNodos;//Establezo el valor del atributo del grafo con lo que lei

	list<Nodo>::iterator it;//Iterator para recorrer la lista, es un apuntador

	for(int i=0;i<NNodos;i++)//Crea una lista del tamaño del numero de esquinas inicializando sus identificadores pero seteando el costo y duracion de semaforo en -1
	{
		Esquinas.push_back(Nodo(i,-1,-1));
	}

	for(int i= 0;i<NAristas;i++)//Agrega a la esquina correspondiente informacion a cerca de su esquina adyacente y su arista incidente
	{
		it=Esquinas.begin();
		
		cin>>Inicio;
		cin>>Final;
		cin>>Valor;
		cin>>Choque;

		for(int i=0;i<Inicio;i++)
			it++;

		it->Adyacentes.push_back(NodosAdyacentes(Final,Valor,Choque));
	}

	it=Esquinas.begin();

	for(int i=0;i<NumNodos;i++)//Lee la duracion de los semaforos y la asigna a la esquina correspondiente
	{
		cin>>DurSemaforo;
		it->DuracionSemaforo=DurSemaforo;
		it++;
	}

	cin>>NodoInicio;//Lee el punto de partida
	cin>>NodoFinal;//Lee el punto de llegada
}

bool Grafo::SemaforoVerde(int TiempoAcumulado,int DurSemaforo)
{
	if((TiempoAcumulado/DurSemaforo)%2 == 0)//Si el tiempo acumulado div duracion del semaforo es par, entonces el semaforo esta en verde
		return true;
	else
		return false;
}

int Grafo::DiferenciaSemaforoRojo(int TiempoAcumulado,int DurSemaforo)
{
	while((TiempoAcumulado/DurSemaforo)%2 != 0)//Mientras el tiempo acumulado div duracion del semaforo no sea par, a mi tiempo acumulado le sumo una unidad
		TiempoAcumulado++;

	return TiempoAcumulado;
}


int Grafo::Minimo(list<Nodo> A)
{
	unsigned long long int Min=9999999999;//Seteo minimo en un numero muy alto
	int NombreNodo=-1;//Inicializo el nombre dle nodo que voy a retornar en -1
	
	list<Nodo>:: iterator it;//Creo mi apuntador de la lista

	for(it=A.begin();it!=A.end();it++)
	{
		if(!it->Visitado)//Si dicha esquina no esta visitada entro
		{
			if((Min>it->Costo) && (it->Costo != -1))//Si mi costo minimo es mayor que el costo de determinada esquina y el costo de la esquina es distinta de -1 que representa el infinito
			{
				Min=it->Costo;//Seteo el minimo como el costo de dicho nodo
				NombreNodo=it->Nombre;//Como retorno el id de la esquina con menor costo seteo la variable de retorno con dicho id
			}
		}
	}
	
	return NombreNodo;
}

void Grafo::Dijkstra(Grafo Graf,int Inicio,int Final)
{
	list<Nodo>:: iterator iter;//Apuntador al nodo final
	list<Nodo>:: iterator Aux1;//Apuntador al nodo actual
	list<Nodo>:: iterator Aux;//Apuntador auxiliar para cuando comparo pesos
	list<NodosAdyacentes>:: iterator Aux2;//Apuntador para moverme por la lista de vertices adyacentes

	int min=0;//Inicializo la variable que contiene el nombre del minimo nodo

	Aux1= Graf.Esquinas.begin();//Inicializo el apuntador de nodos actuales a la primera posicion de la lista de nodos

	for(int i=0;i<Inicio;i++)
		Aux1++;	//Aumento el Apuntador hasta colocarme en el nodo inicio

	Aux1->Costo=0;//Coloco el costo del nodo inicio en 0

	iter= Graf.Esquinas.begin();//Inicializo el apuntador en el que se guardara la posicion del nodo final

	for(int i=0;i<Final;i++)
		iter++;	//Aumento el indice del apuntador hasta llegar al nodo final
	
	while(!iter->Visitado && min != -1)//Condicion de parada de mi busqueda, el nodo final no debe formar parte del conjunto de visitados para entrar al ciclo y el valor retornado a la hora de localizar el minimo debe ser distinto de -1, de presentarse este ultimo caso quiere decir que no se puede llegar hasta el nodo destino
	{
		Aux1=Graf.Esquinas.begin();//Inicializo mi variable de nodo actual en el primer elemento de mi lista de nodos
		min=Minimo(Graf.Esquinas);//LLamo a la subrutina Minimo que me indica por cual nodo debo comenzar basado en el costo

		if(min != -1)
		{		
			for(int i=0;i<min;i++)
				Aux1++;	//Incremento mi variable apuntador hasta llegar al nombre del nodo minimo

			Aux1->Visitado=true;//Marco mi nodo actual como visitado

			for(Aux2=Aux1->Adyacentes.begin();Aux2!=Aux1->Adyacentes.end();Aux2++)//Recorro todos los nodos adyacentes a mi nodo actual
			{
				Aux=Graf.Esquinas.begin();//Auxiliar para actualizar el costo de los nodos en caso de ser neecsario
			
				for(int i=0;i<Aux2->Nombre;i++)//Incremento mi apuntador hasta llegar al nombre del nodo adyacente a mi nodo actual
					Aux++;
			
				if(!Aux->Visitado)//Si dicho nodo adyacente a mi nodo actual no esta visitado
				{
					if(Aux2->Choque==-1)//Si no ocurrira nunca un choque en dicho nodo adyacente
					{
						if(!SemaforoVerde(Aux1->Costo+Aux2->Peso,Aux->DuracionSemaforo))//Si el semaforo no esta en verde con mi peso actual mas el peso de mi arista
						{
							if((DiferenciaSemaforoRojo(Aux1->Costo+Aux2->Peso,Aux->DuracionSemaforo) < Aux->Costo) || (Aux->Costo == -1))//Si el peso de lo que le agrego con el semaforo en rojo es menor que el peso dle nodo adyacente, entonces se modifica o si este esta en infinito
								Aux->Costo=DiferenciaSemaforoRojo(Aux1->Costo+Aux2->Peso,Aux->DuracionSemaforo);//Modificacion de la variable del nodo adyacente al nuevo costo
							
						}else if((Aux1->Costo)+(Aux2->Peso)< (Aux->Costo) || (Aux->Costo == -1))//Si es semaforo no esta en roro y Si el Costo acumulado + el peso de la calle es menor que el peso actual del nodo adyacente
							Aux->Costo=Aux1->Costo+Aux2->Peso;//Actualizo el costo del nodo adyacente
						
					}else if(Aux1->Costo< Aux2->Choque)//Si no ha ocurrido un choque
					{	
						if(!SemaforoVerde(Aux1->Costo+Aux2->Peso,Aux->DuracionSemaforo))//Si el semaforo no esta en verde para cuando haya recorrido la calle
						{
							if((DiferenciaSemaforoRojo(Aux1->Costo+Aux2->Peso,Aux->DuracionSemaforo) < Aux->Costo) || (Aux->Costo == -1))//Si el costo agregado por el semaforo en rojo es menor que el costo actual del nodo adyacente o este tiene peso infinito, entonces modifico el costo de dicha esquina 
								Aux->Costo=DiferenciaSemaforoRojo(Aux1->Costo+Aux2->Peso,Aux->DuracionSemaforo);//Modifico el costo de la esquina
							
						}else if((Aux1->Costo)+(Aux2->Peso)< (Aux->Costo)|| (Aux->Costo == -1))//Si el semaforo no esta en rojo y Si el Costo acumulado + el peso de la calle es menor que el peso actual del nodo adyacente
							Aux->Costo=Aux1->Costo+Aux2->Peso;//Actualizo el costo del nodo adyacente
					}
				}

			}

		}

	}

		cout<<iter->Costo<<endl;//Imprimo el costo de la esquina destino
}


int main()
{
	int Vertices,Aristas,Casos;//Declaracion de variables a ser leidas
	
	Grafo A = Grafo();//Se crea una instancia de la clase Grafo

	cin>>Casos;//Se leen el numero de casos de entrada

	for(int i=0;i<Casos;i++)//Mientras no se hayan procesado todos los casos
	{
		A=Grafo();//Se inicializa la instancia de la clase grafo ocmo un grafo nuevo para reestablecer la integridad de las variables
		
		cin>>Vertices;//Lectura del numero de Esquinas del caso
		cin>>Aristas;//Lesctura del numero de aristas del caso

		A.AgregarGrafo(Vertices,Aristas);//Se cargan los datos invocando al metodo de la clase Grafo: AgregarGrafo

		A.Dijkstra(A,NodoInicio,NodoFinal);//Se procesan los datos obetenidos para emitir una respuesta
	}
	
		return 0;//Fin de programa
}