/*
 * COP 3502H PA1
 * This program is written by: Sahil Narsinghani
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char STATUS_CAT[3][20] = {"ADOPTED", "PENDING", "AVAILABLE"};

typedef struct Cat {
  char *name;
  int age;
  float weight;
  char *breed;
  int status;
} Cat;

typedef struct Kennel {
  char *location;
  int occupancy;
  Cat **cats;
  int maxCapacity;
} Kennel;

typedef struct CatStore {
  int **capacities;
  int numKennels;
  Kennel *kennels;
} CatStore;


char **readBreeds(int *count);
char *getCharPtrByBreed(char **dictionary, char *breedName, int breedCount);
CatStore *createStore(int kennelCount, int breedCount, char ** dictionary);
Kennel* createKennels(int **constraints, int kennelCount, int breedCount, char **dictionary);
Cat **createCats(char **dictionary, int breedCount, int count);
Cat* createSingleCat(char **dictionary, int breedCount);
int canMoveTo(CatStore *s, char *location, char *breed, char **dictionary, int breedCount);
Kennel *getKennelByCat(CatStore *s, Cat *cat );
int getCatPosi(Kennel *home, Cat *cat);
Cat *getCatByName(CatStore *s, char *catName);
void removeCatFromKennel(Kennel *k, Cat *cat);
void runQueries(CatStore *s, char **dictionary, int breedCount, int numQueries);
void freeBreeds(char **dictionary, int breedCount);
void freeStore(int count, CatStore *store);

int main() {

  int breedCount;
  char **dictionary = readBreeds(&breedCount);

  int kennelCount;
  scanf("%d", &kennelCount);

  CatStore *store = createStore(kennelCount, breedCount, dictionary);

  int numQueries;
  scanf("%d", &numQueries);

  runQueries(store, dictionary, breedCount, numQueries);

  freeStore(kennelCount, store);
  freeBreeds(dictionary, breedCount);

  return 0;
} 

/*
 * readBreeds function - reads inputted breed names and
 * stores them dynamically in a string array. 
*/

char **readBreeds(int *count) {
  char temp[100];
  int numBreeds;

  scanf("%d", &numBreeds);
  *count = numBreeds;

  char ** dictionary = (char **)malloc(numBreeds * sizeof(char *));

  for (int i = 0; i < numBreeds; ++i) {
    scanf("%s", temp);
    dictionary[i] = (char *)malloc((strlen(temp) + 1) * sizeof(char));
    strcpy(dictionary[i], temp);
  }

  return dictionary;
}

/*
 * getCharPtrByBreed function - looks for breed name in dictionary
 * and returns pointer to the name's string.
*/

char *getCharPtrByBreed(char **dictionary, char *breedName, int breedCount) {
  for (int i = 0; i < breedCount; ++i) {
    if (strcmp(dictionary[i], breedName) == 0) {
      return dictionary[i];
    }
  }
  return NULL;
}

/*
 * createStore function - creates a CatStore struct and 
 * adds all kennels to it. 
*/

CatStore *createStore(int kennelCount, int breedCount, char ** dictionary) {
  CatStore *store = (CatStore *)malloc(sizeof(CatStore));
  store->numKennels = kennelCount;
  store->capacities = (int **)malloc(kennelCount * sizeof(int *));

  for (int i = 0; i < kennelCount; ++i) {
    store->capacities[i] = (int *)malloc(breedCount * sizeof(int));

    for (int j = 0; j < breedCount; ++j) {
      scanf("%d", &store->capacities[i][j]);
    }
  }

  store->kennels = createKennels(store->capacities, kennelCount, breedCount, dictionary);
  return store;

} 

/*
 * createKennels function - creates array of kennel structs and 
 * adds puts in inputted data into the kennels;
*/

Kennel *createKennels(int **constraints, int kennelCount, int breedCount, char **dictionary) {
  char temp[100];
  Kennel *kennels = (Kennel *)malloc(kennelCount * sizeof(Kennel));

  for (int i = 0; i < kennelCount; ++i) {
    scanf("%s", temp);
    kennels[i].location = (char * )malloc((strlen(temp) + 1) * sizeof(char));
    strcpy(kennels[i].location, temp);

    kennels[i].maxCapacity = 0;
    for (int j = 0; j < breedCount; ++j) {
      kennels[i].maxCapacity += constraints[i][j];
    }

    int numCats;
    scanf("%d", &numCats);
    kennels[i].occupancy = numCats;

    kennels[i].cats = createCats(dictionary, breedCount, numCats);
  }

  return kennels;
}

/*
 * createCats function - creates array of pointrs to Cat structs
 * and takes in inputted data for each cat. 
*/

Cat **createCats(char **dictionary, int breedCount, int count) {
  if (count == 0) {
    return NULL;
  }

  Cat **cats = (Cat **)malloc(count * sizeof(Cat *));
  for (int i = 0; i < count; ++i) {
    cats[i] = createSingleCat(dictionary, breedCount);
  }

  return cats;
}

/*
 * createSingleCat function - creates a single Cat struct and
 * uses user's input to fill in components of Cat struct.
*/

Cat *createSingleCat(char **dictionary, int breedCount) {
  char tempName[100];
  char tempBreed[100];
  int age = 0;
  float weight = 0.0;

  Cat *newCat = (Cat *)malloc(sizeof(Cat));

  scanf("%s %d %f %s", tempName, &age, &weight, tempBreed);
  newCat->name = (char *)malloc((strlen(tempName) + 1) * sizeof(char));
  strcpy(newCat->name, tempName);

  newCat->age = age;
  newCat->weight = weight;

  newCat->breed = getCharPtrByBreed(dictionary, tempBreed, breedCount);
  newCat->status = 2;

  return newCat;
}

/*
 * canMoveTo function - checks if cat of a breed type can move to 
  * another kennel. 
*/

int canMoveTo(CatStore *s, char *location, char *breed, char **dictionary, int breedCount) {
  Kennel *kennel = NULL;
  int kennelIndex = -1;

  for (int i = 0; i < s->numKennels; ++i) {
    if (strcmp(s->kennels[i].location, location) == 0) {
      kennel = &s->kennels[i];
      kennelIndex = i;
      break;
    }
  }

  if (kennel == NULL) {
    return 0;
  }

  int breedIndex = -1;
  for (int i = 0; i < breedCount; ++i) {
    if(strcmp(dictionary[i], breed) == 0) {
      breedIndex = i;
      break;
    }
  }

  if (breedIndex == -1 || kennel->occupancy >= kennel->maxCapacity) {
    return 0;
  }

  int currentBreedCount = 0;
  for (int i = 0; i < kennel->occupancy; ++i) {
    if (strcmp(kennel->cats[i]->breed, breed) == 0) {
      currentBreedCount++;
    }
  }

  if (currentBreedCount >= s->capacities[kennelIndex][breedIndex]) {
    return 0;
  }

  return 1;
}

/*
 * getKenneByCat function - finds and returns kennel where 
 * the cat is. 
*/

Kennel *getKennelByCat(CatStore *s, Cat *cat) {
  for (int i = 0; i < s->numKennels; ++i) {
    for (int j = 0; j < s->kennels[i].occupancy; ++j) {
      if (s->kennels[i].cats[j] == cat) {
        return &s->kennels[i];
      }
    }
  }
  return NULL;
}

/*
 * getCatPosi function - returns index of cat in the kennel cat array.
*/

int getCatPosi(Kennel *home, Cat *cat) {
  for (int i = 0; i < home->occupancy; ++i) {
    if (home->cats[i] == cat) {
      return i;
    }
  }
  return -1;
}

/*
 * getCatByName function - finds and returns pointer to cat struct
 * by looking for cat name. 
*/

Cat *getCatByName(CatStore *s, char *catName) {
  for (int i = 0; i < s->numKennels; ++i) {
    for (int j = 0; j < s->kennels[i].occupancy; ++j) {
      if (strcmp(s->kennels[i].cats[j]->name, catName) == 0) {
        return s->kennels[i].cats[j];
      }
    }
  }
  return NULL;
}

/*
 * removeCatFromKennel function - removes cat from kennel's cat array
 * and shifts other cats index positions after removing cat. 
*/


void removeCatFromKennel(Kennel *k, Cat *cat) {
  int catToRemoveIndex = getCatPosi(k, cat);
  if (catToRemoveIndex == -1) {
    return;
  }

  for (int i = catToRemoveIndex; i < k->occupancy - 1; ++i) {
    k->cats[i] = k->cats[i + 1];
  }

  k->occupancy--;

  if (k->occupancy > 0) {
    Cat **temp = (Cat **)realloc(k->cats, k->occupancy * sizeof(Cat *));
    if (temp != NULL) {
      k->cats = temp;
    } 
  }
  else {
    free(k->cats);
    k->cats = NULL;
  }
}

/*
 * runQueries function - runs all user's inputted queries based on
 * type of query. 
*/

void runQueries(CatStore *s, char **dictionary, int breedCount, int numQueries) {
  for (int i = 0; i < numQueries; ++i) {
    int queryType;
    scanf("%d", &queryType);

    if (queryType == 1) {
      char breed[100];
      scanf("%s", breed);
    
      int catFound = 0;
      for (int j = 0; j < s->numKennels; ++j) {
        for (int k = 0; k < s->kennels[j].occupancy; ++k) {
          if (strcmp(s->kennels[j].cats[k]->breed, breed) == 0) {
            printf("%s %.2f %d %s %s\n", s->kennels[j].cats[k]->name, s->kennels[j].cats[k]->weight, s->kennels[j].cats[k]->age, s->kennels[j].location, STATUS_CAT[s->kennels[j].cats[k]->status]);
            catFound = 1;
          }
        }
      }
      if (catFound == 0) {
        printf("No cat with breed %s\n", breed);
      } 
  } else if (queryType == 2) {
        int newStatus;
        char name[100];
        scanf("%d %s", &newStatus, name);

        Cat *cat = getCatByName(s, name);
        if (cat != NULL) {
          cat->status = newStatus;
          printf("%s is now %s!\n", cat->name, STATUS_CAT[newStatus]);

          if (newStatus == 0) {
            Kennel *kennel = getKennelByCat(s, cat);
            
            if (kennel != NULL) {
              removeCatFromKennel(kennel, cat);
              free(cat->name);
              free(cat);
            }
          }
        } 
       } 
    else if (queryType == 3) {
          char name[100];
          char catLocation[100];
          scanf("%s %s", name, catLocation);

          Cat *cat = getCatByName(s, name);
          if (cat == NULL) {
            continue;
          }

          Kennel *destination = NULL;
          for (int j = 0; j < s->numKennels; ++j) {
            if (strcmp(s->kennels[j].location, catLocation) == 0) {
              destination = &s->kennels[j];
              break;
            }
          }
          if (destination == NULL) {
            printf("No location for cat found: %s\n", catLocation);
            continue;
          }

          if (canMoveTo(s, catLocation, cat->breed, dictionary, breedCount) == 0) {
            printf("%s cannot take a %s cat!\n", catLocation, cat->breed);
            continue;
          }

          Kennel *currentKennel = getKennelByCat(s, cat);
          if (currentKennel == NULL) {
            continue;
          }

          removeCatFromKennel(currentKennel, cat);

          destination->occupancy++;
          Cat **temp = (Cat **)realloc(destination->cats, destination->occupancy * sizeof(Cat *));

          if (temp != NULL) {
            destination->cats = temp;
            destination->cats[destination->occupancy - 1] = cat;
          }
          printf("%s moved successfully to %s\n", cat->name, destination->location);  
        }
      }
    }

/*
 * freeBreeds function - frees all memory used
 * for breed dictionary. 
*/

void freeBreeds(char **dictionary, int breedCount) {
  for (int i = 0; i < breedCount; ++i) {
    free(dictionary[i]);
  }
  free(dictionary);
}

/*
 * freeStore function - frees all memory used
 * for CatStore struct and components. 
*/

void freeStore(int count, CatStore *store) {
  for (int i = 0; i < count; ++i) {
    for (int j = 0; j < store->kennels[i].occupancy; ++j) {
      free(store->kennels[i].cats[j]->name);
      free(store->kennels[i].cats[j]);
    }
    free(store->kennels[i].cats);
    free(store->kennels[i].location);
    free(store->capacities[i]);
  }
  free(store->capacities);
  free(store->kennels);
  free(store);
}
