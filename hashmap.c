#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value)
{
  if (map == NULL || key == NULL)
    return;

  if ((map->size) >= (map->capacity * 0.7))
    enlarge(map);
  
  long posicion = hash(key, map->capacity);
  Pair * pair = searchMap(map, key);

  if (pair == NULL)
  {
    while (posicion < map->capacity)
    {
      if (map->buckets[posicion] == NULL)
      {
        map->buckets[posicion] = createPair(key, value);
        map->current = posicion;
        map->size = map->size + 1;
        return;
      }
      posicion = (posicion + 1) % map->capacity;
    }
  }

  else
  {
    pair->value = value;
    
    while (posicion < map->capacity)
    {
      if (is_equal(pair->key, key) == 1)
        break;
      
      posicion = (posicion + 1) % map->capacity;
    }
    map->current = posicion;
    return;
  }
}

void enlarge(HashMap * map)
{
  //enlarge_called = 1; //no borrar (testing purposes)
  if (map == NULL)
    return;

  Pair ** aux = map->buckets;
  map->capacity = map->capacity * 2;

  map->buckets = (Pair **)calloc(map->capacity, sizeof(Pair *));

  map->size = 0;

  for (long i = 0; i < (map->capacity / 2); i++)
  {
    if (aux[i] != NULL)
    {
      insertMap(map, aux[i]->key, aux[i]->value);
      free(aux[i]);
    }
  }

  free(aux);
}


HashMap * createMap(long capacity)
{
  HashMap * map = (HashMap *)malloc(sizeof(HashMap));
  map->buckets = (Pair **)calloc(capacity, sizeof(Pair *));

  map->size = 0;
  map->capacity = capacity;
  map->current = -1;
  
  return map;
}

void eraseMap(HashMap * map,  char * key)
{    
  Pair * elem = searchMap(map, key);

  if (elem == NULL)
    return;

  elem->key = NULL;
  elem->value = NULL;
  map->size = map->size - 1;
}

Pair * searchMap(HashMap * map,  char * key)
{   
  if (map == NULL || key == NULL)
    return NULL;

  long posicion = hash(key, map->capacity);

  if (map->buckets[posicion] == NULL || map->buckets[posicion]->key == NULL)
  {
    return NULL;
  }

  else
  {
    while (map->buckets[posicion] != NULL && map->buckets[posicion]->key != NULL)
    {
      if (is_equal(map->buckets[posicion]->key, key) == 1)
      {
        map->current = posicion;
        return map->buckets[posicion];
      }
      
      posicion = (posicion + 1) % map->capacity;
    }
    return NULL;
  }
}

Pair * firstMap(HashMap * map)
{
  if (map == NULL)
    return NULL;
  
  long posicion = 0;

  while (posicion < map->capacity)
  {
    if (map->buckets[posicion] != NULL && map->buckets[posicion]->key != NULL)
    {
      map->current = posicion;
      return map->buckets[posicion];
    }
    posicion = (posicion + 1) % map->capacity;
  }
  
  map->current = -1;
  return NULL;
}

Pair * nextMap(HashMap * map)
{
  if (map == NULL)
    return NULL;

  for (long i = (map->current + 1); i < map->capacity; i++)
  {
    if (map->buckets[i] != NULL)
    {
      map->current = i;
      return map->buckets[i];
    }
  }

  return NULL;
}