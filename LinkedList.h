#ifndef LINKEDLIST_H_INCLUDED
#define LINKEDLIST_H_INCLUDED


/**
 * @author Aurélien BLAISE
 * @date december 2018
 * @version 1.0
 * @brief Brings together some useful functions to handle the linked lists
 */



/* ========== STRUCTURES ========== */
/**
 * @struct Bullet
 * @brief Stands for an element of the linked list, here the data is a Bullet
 */
typedef struct Bullet Bullet;
struct Bullet
{
    SDL_Rect position;  /**< Position of the bullet */
    int side;           /**< Side where the character is firing */
    Bullet* next;       /**< Pointer on the next bullet in the linked list */
};

/**
 * @struct LinkedList
 * @brief Handles the linked list, it's the entry point of the list
 */
typedef struct LinkedList
{
    Bullet* first; /**< Pointer on the first bullet of the linked list */
    int nbBullets; /**< Number of bullets of the linked list */
}LinkedList;


/* ========== FUNCTIONS ========== */
/**
 * @brief Create a new linked list and initialise it
 *
 * @return < LinkedList* > the linked list created
 */
LinkedList* list_initialise(int x, int y, int side);

/**
 * @brief Append to the linked list a new bullet which has to be the first
 *
 * @param < *list > LinkedList in which the element has to be appended in first position
 * @param < position > position of the bullet which has to be appended
 */
void list_append_first(LinkedList* list, SDL_Rect position, int side);

/**
 * @brief Seek if the linked list contains the desired bullet
 *
 * @param < *list > LinkedList in which the bullet has to be sought
 * @param < *bullet > bullet which has to be sought
 * @return < int > 1 if the list actually contains the desired bullet, 0 if not
 */
int list_contain(LinkedList *list, Bullet *bullet);

/**
 * @brief Return a bullet at the desired index
 *
 * @param < *list > LinkedList in which the element has to be returned
 * @param < index > desired index of the bullet to return
 * @return < Bullet* > element that is returned at the proper index
 */
Bullet* list_return_bullet(LinkedList *list, int index);

/**
 * @brief Print the whole linked list
 *
 * @param < *list > LinkedList which has to be printed
 */
void list_print(LinkedList *list);

/**
 * @brief Delete the first bullet of the linked list
 *
 * @param < *list > LinkedList whose the first bullet has to be deleted
 */
void list_delete_first(LinkedList *list);

/**
 * @brief Delete a bullet of the linked list
 *
 * @param < *list > LinkedList in which a bullet has to be deleted
 * @param < *bullet > desired bullet which has to be deleted
 */
void list_delete_element(LinkedList *list, Bullet *bullet);

/**
 * @brief Delete the whole linked list
 *
 * @param < *list > LinkedList which has to be deleted
 */
void list_delete(LinkedList *list);


#endif // LINKEDLIST_H_INCLUDED
