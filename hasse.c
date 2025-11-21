#include "hasse.h"
#include <stdlib.h>
#include "markov.h"



void removeTransitiveLinks(t_liens_array *p_liens_array)
{
    int i = 0;
    while (i < p_liens_array->nb_liens)
    {
        t_lien link1 = p_liens_array->liens[i];
        int j = 0;
        int to_remove = 0;
        while (j < p_liens_array->nb_liens && !to_remove)
        {
            if (j != i)
            {
                t_lien link2 = p_liens_array->liens[j];
                if (link1.classe_depart == link2.classe_depart)
                {
                    int k = 0;
                    while (k < p_liens_array->nb_liens && !to_remove)
                    {
                        if (k != j && k != i)
                        {
                            t_lien link3 = p_liens_array->liens[k];
                            if ((link3.classe_depart == link2.classe_arrivee) &&
                                (link3.classe_arrivee == link1.classe_arrivee))
                            {
                                to_remove = 1;
                            }
                        }
                        k++;
                    }
                }
            }
            j++;
        }
        if (to_remove)
        {
            p_liens_array->liens[i] = p_liens_array->liens[p_liens_array->nb_liens - 1];
            p_liens_array->nb_liens--;
        }
        else
        {
            i++;
        }
    }
}
