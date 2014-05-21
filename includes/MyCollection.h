/* 
 * File:   MyCollection.h
 * Author: artyom
 *
 * Created on 13 Ноябрь 2011 г., 17:13
 */

#ifndef MYCOLLECTION_H
#define	MYCOLLECTION_H
#include "stddef.h"
#include "complect_headers.h"

template <class T>
struct ITEM
{
    char * key;
    T item;
    ITEM(const ITEM& IT)
    {
        key = (char*)malloc(strlen(IT.key)+1);
        strcpy(key,IT.key);
        item =IT.item;
    }
    ITEM(char * k,const T& it)
    {
        key = (char*)malloc(strlen(k)+1);
        strcpy(key,k);
        item = it;
    }

    ~ITEM()
    {
        free(key);
    }
};

template <class T>
class MyCollection
{
private:
    int capacity,len;
    int iterator_index;
    ITEM<T> ** items;
public:

    MyCollection()
    {
        iterator_index = len = 0;
        items = new ITEM<T>*[capacity = 5];
    }
    ~MyCollection()
    {
        if (items == NULL) return;
        for (int i = 0; i<len; i++) delete items[i];
        delete items;
    }
    MyCollection(MyCollection &coll)
    {
        iterator_index = len = 0;
        items = new ITEM<T>*[capacity = (coll.Lenght()+3)];
        ITEM<T> * it;
        if ((it = coll.getFirst())!=NULL)
            do
            {
                items[len++] = new ITEM<T>(*it);
            } while ((it = coll.getNext())!=NULL);

    }
    int Lenght()
    {
        return len;
    }
    MyCollection<T>& operator = (MyCollection &coll)
    {
        Clear();
        delete items;
        iterator_index = len = 0;
        items = new ITEM<T>*[capacity = (coll.Lenght()+3)];
        ITEM<T> * it;
        if ((it = coll.getFirst())!=NULL)
            do
            {
                items[len++] = new ITEM<T>(*it);
            } while ((it = coll.getNext())!=NULL);
        return *this;
    }

    ITEM<T> * getFirst()
    {
        iterator_index = 0;
        if (len == 0) return NULL;
        return items[0];
    }
    ITEM<T> * getNext()
    {
        if (iterator_index == len-1) return NULL;
        return items[++iterator_index];
    }

    T& operator[] (char * key)
    {
        int ind = binaryFind(key);
        if (ind<0) return items[0]->item;
        if (ind==len-1) return items[len-1]->item;
        if (strcmp(key,items[ind]->key)==0) return items[ind]->item;
        int i0 = 0,i1 = 0, ind1=ind+1;
        while (key[i0]==items[ind]->key[i0]) i0++;
        while (key[i1]==items[ind1]->key[i1]) i1++;
        if (i1>i0) return items[ind1]->item;
        return items[ind]->item;
    }

    bool Exists(char * Key)
    {
        int ind = binaryFind(Key);
        if (ind == -1 || strcmp(Key,items[ind]->key)) return false;
        return true;
    }
    T DeleteItem(char * Key)
    {
        if (len == 0) return NULL;
        if (len == capacity/4) set_capacity(capacity/2);
        int ind = binaryFind(Key);
        if (ind==-1) ind = 0;
        T ret = items[ind]->item;
        delete items[ind];
        len--;
        for (int i = ind; i<len; i++) items[i] = items[i+1];
        return ret;
    }

    void Clear()
    {
        for (int i = 0; i<len; i++) delete items[i];
        len = iterator_index = 0;
        set_capacity(5);
    }

    void AddItem(char *Key,const T& Item)
    {
        if (len == capacity) set_capacity(capacity*2);
        int ind = binaryFind(Key);
        if (ind == -1 || strcmp(Key,items[ind]->key))
        {
            for (int i = len; i>ind; i--) items[i] = items[i-1];
            len++;
            ind++;
            items[ind] = new ITEM<T>(Key,Item);
        }
        else items[ind]->item = Item;
    }
    ITEM<T> * currentItem()
    {
        if (iterator_index<0 || iterator_index>=len) return NULL;
        return items[iterator_index];
    }

    inline void AddItem(ITEM<T> * item)
    {
        AddItem(item->key,item->item);
    }

    inline void AddItem(wchar_t *Key,const T& Item)
    {
        int l = 0;
        while (Key[l++]);
        char * key = (char*)malloc(l);
        while (l--) key[l] = Key[l];
        AddItem(key,Item);
        free(key);
    }

private:
    void set_capacity(int C)
    {
        if (C<=0 || C == capacity) return;
        ITEM<T> ** new_items = new ITEM<T>*[capacity = C];
        if (items!=NULL)
        {
            int m_len = len<C?len:C;
            for (int i = 0; i<m_len; i++) new_items[i] = items[i];
            delete items;
        }
        items = new_items;
    }


    int binaryFind(char * key)
    {
        if (len == 0 || strcmp(key,items[0]->key)<0) return -1;
        int x1 = 0, x2 = len, x = len>>1, res_comp;

        while (x!=x1)
        {
            res_comp = strcmp(key,items[x]->key);
            if (res_comp == 0) return x;
            else if (res_comp>0) x1 = x;
            else x2 = x;
            x = (x1 + x2)>>1;
        }
        return x;
    }


};


#endif	/* MYCOLLECTION_H */

