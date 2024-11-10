#include "set.h"

/**
 * This function initializes a set without an element and returns it.
 */
Set initSet() {
    
    Set set = (Set)malloc(sizeof(Set));

    set->elements = NULL;
    set->cardinality = 0;

    return set;
}

/**
 * This function creates an integer-typed element and returns it. 
 * It allocates memory for both the element and the integer.
 */
Element createIntegerElement(int data) {
   
    Element element = (Element)malloc(sizeof(Element_t));

   int* integerData = (int*)malloc(sizeof(int));
   
    *integerData = data;      
    element->type = INTEGER;  
    element->data = integerData;

    return element;
}


/**
 * This function creates a float-typed element and returns it. 
 * It allocates memory for both the element and the float.
 */
Element createFloatElement(float data) {
    
    Element element = (Element)malloc(sizeof(Element));

    float* floatData = (float*)malloc(sizeof(float));
   
    *floatData = data;       
    element->type = FLOAT;    
    element->data = floatData; 

    return element;
}


/**
 * This function creates a string-typed element and returns it. 
 * It allocates memory for both the element and the string.
 * This function uses the `strlen` and the `strcpy` functions.
 */
Element createStringElement(char* data) {
   
    Element element = (Element)malloc(sizeof(Element));
  
    char* stringData = (char*)malloc((strlen(data)) * sizeof(char));
    
    strcpy(stringData, data);

    element->type = STRING;
    element->data = stringData;

    return element;
}
/**
 * This function creates a matrix-point-typed element and returns it. 
 * It allocates memory for both the element and the matrix point.
 */
Element createMatrixPointElement(int x, int y, int data) {
    
    Element element = (Element)malloc(sizeof(Element_t));
    
    int* pointData = (int*)malloc(3 * sizeof(int));
    
    pointData[0] = x;   // row
    pointData[1] = y;      // column
    pointData[2] = data;   // value 

    element->type = MATRIX_POINT;
    element->data = pointData;

    return element;
}

/**
 * This function compares two elements to see if they are identical. 
 * First, it checks if the types are identical. If they are the same, 
 * it checks if the data are the same. However, this function does not 
 * compare addresses because the value from two different addresses 
 * can be the same. It returns one if the elements are the same; 
 * otherwise, it returns zero. This function uses the `strcmp` function.
 */
int isSame(Element e1, Element e2) {
    if (e1->type != e2->type) {
        return 0;
    }

    switch (e1->type) {
        case INTEGER:
            return *((int*)e1->data) == *((int*)e2->data);

        case FLOAT:
            return *((float*)e1->data) == *((float*)e2->data);

        case STRING:
				// if two string are equal strcmp returns 0!!
            return strcmp((char*)e1->data, (char*)e2->data) == 0;

        case MATRIX_POINT: {
            int* point1 = (int*)e1->data;
            int* point2 = (int*)e2->data;
            return (point1[0] == point2[0]) && // x match
                   (point1[1] == point2[1]) && // y match
                   (point1[2] == point2[2]);   // data values match
        }

        default:
            return 0; // Unsupported  enum trype
    }
}

/**
 * This function checks if the given element is in the set or not. 
 * It does not compare the addresses. It returns one if the given 
 * element is in the set; otherwise, it returns zero.
 */
int in(Set set, Element element) {
    
    for (int i = 0; i < set->cardinality; i++) {
        if (isSame(set->elements[i], element)) {
            return 1;
        }
    }
    return 0;
}

/**
 * This function inserts an element into the given set. 
 * The same element cannot be twice in the same set.
 * It returns one if the inserting element is successful; 
 * otherwise, it returns zero. This function uses the 
 * `realloc` function.
 */
int insertElement(Set set, Element element) {
    // element is already in
    if (in(set, element)) {
        return 0;
    }
    Element* newElements = (Element*)realloc(set->elements, (set->cardinality + 1) * sizeof(Element));
    
    // assign reallocated memory back
    set->elements = newElements;
    set->elements[set->cardinality] = element;
    set->cardinality += 1;

    return 1;
}

/**
 * This function removes the given element in the set. 
 * It returns one if removal is successful; otherwise, 
 * it returns zero. This function uses the `realloc` function.
 */
int removeElement(Set set, Element element) {
    // element not in
    if (!in(set, element)) {
        return 0;
    }

    int index = -1;
    for (int i = 0; i < set->cardinality; i++) {
        if (isSame(set->elements[i], element)) {
            index = i;
            break;
        }
    }
    // free that index
    free(set->elements[index]);

    // shift next elements for filling the empty gap
    for (int i = index; i < set->cardinality - 1; i++) {
        set->elements[i] = set->elements[i + 1];
    }

    Element* newElements = (Element*)realloc(set->elements, (set->cardinality - 1) * sizeof(Element));
  

    // assign reallocated memory back
    set->elements = newElements;
    set->cardinality -= 1;

    return 1; // Removal successful
}

/**
 * This function creates and returns a new set, which is united of the given sets.
 */
Set unite(Set s1, Set s2) {
  // for toring the union set
    Set unionSet = initSet();
    

   // first, insert all elements on s1.
    for (int i = 0; i < s1->cardinality; i++) {
        // duplicate check
        if (!in(unionSet, s1->elements[i])) { 
            insertElement(unionSet, s1->elements[i]);
        }
    }

    // insert s2. 
    for (int i = 0; i < s2->cardinality; i++) {
        //check duplicates after adding s1 and itself
        if (!in(unionSet, s2->elements[i])) { 
            insertElement(unionSet, s2->elements[i]);
        }
    }
    return unionSet;
}

/**
 * This function creates and returns a new set, which is intersected of the given sets.
 */
Set intersect(Set s1, Set s2) {
    // for storing interaction set 
    Set intersectionSet = initSet();
   
    // checking same values with s2
    for (int i = 0; i < s1->cardinality; i++) {
        if (in(s2, s1->elements[i])) {
            insertElement(intersectionSet, s1->elements[i]);
        }
    }
    return intersectionSet;
}

/**
 * This function creates and returns a new set, which is 
 * subtracted from the first given set by the second one.
 */
Set substract(Set s1, Set s2) {
    // init substructed set 
    Set substructedSet = initSet();

   // if s1 and s2 element not same insert to set
    for (int i = 0; i < s1->cardinality; i++) {
        if (!in(s2, s1->elements[i])) {
            insertElement(substructedSet, s1->elements[i]);
        }
    }
    return substructedSet;
}

/**
 * This function prints the given element depending on its type.
 */
void printElement(Element element) {
    switch (element->type) {
        case INTEGER:
            printf("%d", *((int*)element->data));
            break;

        case FLOAT:
            printf("%.2f", *((float*)element->data));
            break;

        case STRING:
            printf("%s", (char*)element->data);
            break;

        case MATRIX_POINT: {
            int* pointData = (int*)element->data;
            printf("(x=%d, y=%d, data=%d)\n", pointData[0], pointData[1], pointData[2]);
            break;
        }

        default:
            printf("Unknown type");
    }
}

/**
 * This function prints the given set element by element depending on its type.
 */
void printSet(Set set) {
    if (set->cardinality == 0) {
        printf("Set is empty.\n");
        return;
    }
    for (int i = 0; i < set->cardinality; i++) {
        printElement(set->elements[i]);
    }
}

/**
 * This function deallocates all data in the given set and the set itself.
 */
void freeSet(Set set) {
    // free each element
    for (int i = 0; i < set->cardinality; i++) {
        free(set->elements[i]);
    }
   // free element array pointer
    free(set->elements);
    // free the remainer set
    free(set);
}

