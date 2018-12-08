/**
 * @author Aurélien BLAISE
 * @date december 2018
 * @version 1.0
 * @brief Brings together some useful functions to handle the linked lists
 */


#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "LinkedList.h"


/**
 * @brief Create a new linked list and initialise it
 *
 * @return < LinkedList* > the created linked list
 */
LinkedList* list_initialise(int x, int y, int side)
{
    LinkedList *list = malloc(sizeof(*list));
    Bullet *bullet = malloc(sizeof(*bullet));

    if(list == NULL || bullet == NULL)
    {
        fprintf(stderr, "ERROR : Impossible to initialise the LinkedList\n");
        exit(EXIT_FAILURE);
    }

    bullet->position.w = 10; // 10/3 = 3.33 presque le même rapport que 967/300 les dimensions du sprite d'une balle
    bullet->position.h = 3;
    bullet->position.x = x;
    bullet->position.y = y;
    bullet->side = side;

    bullet->next = NULL;
    list->first = bullet;
    list->nbBullets = 1;

    return list;
}

/**
 * @brief Append to the linked list a new bullet which has to be the first
 *
 * @param < *list > LinkedList in which the element has to be appended in first position
 * @param < position > position of the bullet which has to be appended
 */
void list_append_first(LinkedList* list, SDL_Rect position, int side)
{
    if(list == NULL)
    {
        fprintf(stderr, "ERROR : list not initialised, impossible to append a new bullet\n");
        exit(EXIT_FAILURE);
    }

    Bullet* newBullet = malloc(sizeof(Bullet));
    if(newBullet == NULL)
    {
        fprintf(stderr, "ERROR : Impossible to append a new bullet\n");
        exit(EXIT_FAILURE);
    }

    newBullet->position = position;
    newBullet->side = side;

    newBullet->next = list->first;
    list->first = newBullet;

    list->nbBullets++;
}

/**
 * @brief Seek if the linked list contains the desired bullet
 *
 * @param < *list > LinkedList in which the bullet has to be sought
 * @param < *bullet > bullet which has to be sought
 * @return < int > 1 if the list actually contains the desired bullet, 0 if not
 */
int list_contain(LinkedList *list, Bullet *bullet)
{
    if(list == NULL)
    {
        fprintf(stderr, "ERROR : list not initialised, so it contains nothing\n");
        exit(EXIT_FAILURE);
    }

    Bullet* actualBullet = list->first;

    while(actualBullet != NULL)
    {
        if(actualBullet == bullet)
            return 1;

        actualBullet = actualBullet->next;
    }

    return 0;
}

/**
 * @brief Return a bullet at the desired index
 *
 * @param < *list > LinkedList in which the element has to be returned
 * @param < index > desired index of the bullet to return
 * @return < Bullet* > element that is returned at the proper index
 */
Bullet* list_return_bullet(LinkedList *list, int index)
{
    if(list == NULL)
    {
        fprintf(stderr, "ERROR : list not initialised, impossible to return a bullet\n");
        exit(EXIT_FAILURE);
    }

    if(index >= list->nbBullets)
    {
        fprintf(stderr, "ERROR : index superior than the number of fired bullets in the linked list, impossible to return a bullet\n");
        exit(EXIT_FAILURE);
    }

    Bullet *actualBullet = list->first;

    for(int i = 0; i < index; i++)
    {
        actualBullet = actualBullet->next;
    }

    return actualBullet;
}

/**
 * @brief Print the whole linked list
 *
 * @param < *list > LinkedList which has to be printed
 */
void list_print(LinkedList *list)
{
    int i = 0;

    if(list == NULL)
    {
        fprintf(stderr, "ERROR : List not initialised, impossible to print its elements\n");
        exit(EXIT_FAILURE);
    }

    Bullet *bulletToPrint = list->first;

    while(bulletToPrint != NULL)
    {
        printf("%d SIDE : %d, w = %d, h = %d, x = %d, y = %d\n", i, bulletToPrint->side, bulletToPrint->position.w, bulletToPrint->position.h, bulletToPrint->position.x, bulletToPrint->position.y);
        bulletToPrint = bulletToPrint->next;
        i++;
    }
    printf("\n");
}

/**
 * @brief Delete the first bullet of the linked list
 *
 * @param < *list > LinkedList whose the first bullet has to be deleted
 */
void list_delete_first(LinkedList *list)
{
    if(list == NULL)
    {
        fprintf(stderr, "ERROR : List not initialised, impossible to delete the first bullet\n");
        exit(EXIT_FAILURE);
    }

    if(list->first != NULL)
    {
        Bullet *bulletToDelete = list->first;
        list->first = list->first->next;
        free(bulletToDelete);

        list->nbBullets--;
    }
    else // list == NULL
        fprintf(stderr, "ERROR : List is empty, nothing to delete\n");
}

/**
 * @brief Delete a bullet of the linked list
 *
 * @param < *list > LinkedList in which a bullet has to be deleted
 * @param < *bullet > desired bullet which has to be deleted
 */
void list_delete_element(LinkedList *list, Bullet *bullet)
{
    if(list == NULL)
    {
        fprintf(stderr, "ERROR : List not initialised, impossible to delete a bullet\n");
        exit(EXIT_FAILURE);
    }

    if(!list_contain(list, bullet))
    {
        fprintf(stderr, "ERROR : List doesn't contain the bullet passed in the parameters of the function that it has to be deleted.");
        exit(EXIT_FAILURE);
    }

    int i = 0;
    Bullet *actualBullet = list->first;
    Bullet *previousBullet;

    while(actualBullet != bullet)
    {
        actualBullet = actualBullet->next;
        i++;
    }

    if(i == 0)
    {
        list_delete_first(list);
    }
    else
    {
        previousBullet = list_return_bullet(list, i - 1); // i is the position of bullet to delete, we want the previous one
        previousBullet->next = bullet->next;
        free(bullet);

        list->nbBullets--;
    }
}

/**
 * @brief Delete the whole linked list
 *
 * @param < *list > LinkedList which has to be deleted
 */
void list_delete(LinkedList *list)
{
    if(list == NULL)
    {
        fprintf(stderr, "ERROR : List not initialised, impossible to delete it\n");
        exit(EXIT_FAILURE);
    }

    while(list->first != NULL)
    {
        list_delete_first(list);
    }
    free(list);
}


