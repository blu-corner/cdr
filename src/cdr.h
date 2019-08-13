/*
 * Copyright 2014-2018 Neueda Ltd.
 */
#ifndef _CDR_H_
#define _CDR_H_

#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS
#endif

#include <errno.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include <time.h>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>

#ifdef WIN32
#include "asprintf.h"

#define u_int uint32_t
#define PRINTFLIKE(a, b)
#define gmtime_r(value, tm) gmtime_s(tm, value)
#else
#define PRINTFLIKE(a, b) __attribute__ ((format (printf, a, b)))
#endif // WIN32

namespace neueda
{

using namespace std;

class cdr;

typedef vector<cdr> cdrArray;

typedef int32_t cdrKey_t;

/* Struct for storage of date time within a cdr */
struct cdrDateTime
{
    cdrDateTime () : mHour (0),
                     mMinute (0),
                     mSecond (0),
                     mNanosecond (0),
                     mDay (0),
                     mMonth (0),
                     mYear (0)
    {}

    u_int mHour;
    u_int mMinute;
    u_int mSecond;
    u_int mNanosecond;

    u_int mDay;
    u_int mMonth;
    u_int mYear;
};

enum cdrItemType
{
    CDR_STRING,
    CDR_DOUBLE,
    CDR_INTEGER,
    CDR_DATETIME,
    CDR_ARRAY
};

/**
 * Object used for storing values within the cdr. Items contain both the key and
 * associated value.
 */
struct cdrItem
{
    typedef pair<const cdrKey_t, cdrItem> itemPair;

    cdrItem () : mKey(0),
                 mPair (NULL),
                 mType (CDR_STRING),
                 mDouble (0),
                 mInteger (0)
    {}

    cdrItem (cdrItemType type) : mKey(0),
                                 mPair (NULL),
                                 mType (type),
                                 mDouble (0),
                                 mInteger (0)
    {}

    cdrItem (const cdrItem& other) : mKey (other.mKey),
                                     mIndex (other.mIndex),
                                     mPair (NULL),
                                     mType (other.mType),
                                     mString (other.mString),
                                     mDouble (other.mDouble),
                                     mInteger (other.mInteger),
                                     mDateTime (other.mDateTime),
                                     mArray (other.mArray)
    {}

    ~cdrItem () { delete mPair; }

    cdrItem& operator= (const cdrItem& obj)
    {
        if (this != &obj)
        {
            mKey = obj.mKey;
            mIndex = obj.mIndex;
            mType = obj.mType;
            mString = obj.mString;
            mDouble = obj.mDouble;
            mInteger = obj.mInteger;
            mDateTime = obj.mDateTime;
            mArray = obj.mArray;
        }
        return *this;
    }

    /**
     * Converts the item value to a string and assigns to value.
     * @param value The value to assign to
     * @return Whether the item was successfully converted
     */
    bool asString (string& value) const;

    cdrKey_t     mKey;
    u_int        mIndex;
    itemPair*   mPair;
    cdrItemType mType;
    string      mString;
    double      mDouble;
    int64_t     mInteger;
    cdrDateTime mDateTime;
    cdrArray    mArray;
};

class cdr
{
private:
    typedef map<cdrKey_t, cdrItem> itemMap;
    typedef map<u_int, cdrItem*> orderedItemMap;

public:
    cdr ();

    cdr (const cdr& obj);

    cdr& operator= (const cdr& obj);

    virtual ~cdr() {};

    /**
     * Delete all items in the cdr
     */
    void clear ();

    /**
     * Get the number of items contained within the cdr
     */
    size_t size () const;

    /**
     * Iterates the given cdr and adds any new items to this, any pre-existing
     * items will have the values replaced by those of the given cdr
     *
     * @param sourceCdr the cdr to iterate and apply to this
     */
    void update (cdr& d);

    /**
     * Determine whether a key is present within the cdr
     *
     * @return presence
     */
    bool contains (const cdrKey_t& key) const;

    /**
     * Checks if the item at key is of specified type
     *
     * @param key
     * @param type the type to check
     * @param size if string is provided check whether size matches
     * @return true if type matches
     */
    bool isType (const cdrKey_t& key,
                 cdrItemType type,
                 size_t size = 0) const;

    /**
     * assign the given item to the key
     *
     * @param key
     * @param item
     */
    void setItem (const cdrKey_t& key, cdrItem& item);

    /**
     * retrieve the item at key
     *
     * @param key
     * @return the item found
     */
    const cdrItem* getItem (const cdrKey_t& key) const;

    /**
     * remove the item at the given key
     *
     * @param key
     */
    void deleteItem (const cdrKey_t& key);

    /**
     * Assign the given string value to the key. Also supports formatted string
     * values
     *
     * @param key
     * @param value string to assign
     */
    void setString (const cdrKey_t& key, const string& value);
	void setString(const cdrKey_t& key, const char* fmt, ...) PRINTFLIKE(3, 4);

    /**
     * retrieve the value at the given key as a string. This function will
     * convert the value to a string if it is not of string type
     *
     * @param key
     * @param value string to be assigned to
     * @return whether the given key was present in the map
     */
    bool getString (const cdrKey_t& key, string& value) const;

    /**
     * Assign the given double value to the key.
     * values
     *
     * @param key
     * @param value double to assign to key
     */
    void setDouble (const cdrKey_t& key, double value);

    /**
     * retrieve the value at the given key as a double. If the value is a string
     * it will be converted to double, if that fails the function will return
     * false.
     *
     * @param key
     * @param value double value to assign to the key
     * @return whether the given key was present in the map, and the value
     * was/could be converted to a double.
     */
    bool getDouble (const cdrKey_t& key, double& value) const;

    /**
     * Assign the given integer value to the key.
     * values
     *
     * @param key
     * @param value integer to assign to key
     */
    void setInteger (const cdrKey_t& key, int64_t value);
    
    /**
     * retrieve the value at the given key as a integer. If the value is a 
     * string it will be converted to double, if that fails the function will 
     * return false.
     *
     * @param key
     * @param value integer value to assign to the key
     * @return whether the given key was present in the map, and the value
     * was/could be converted to a double.
     */
    bool getInteger (const cdrKey_t& key, int64_t& value) const;
    bool getInteger (const cdrKey_t& key, uint64_t& value) const;
    bool getInteger (const cdrKey_t& key, int32_t& value) const;
    bool getInteger (const cdrKey_t& key, uint32_t& value) const;
    bool getInteger (const cdrKey_t& key, int16_t& value) const;
    bool getInteger (const cdrKey_t& key, uint16_t& value) const;
    bool getInteger (const cdrKey_t& key, uint8_t& value) const;
    bool getInteger (const cdrKey_t& key, int8_t& value) const;
    bool getInteger (const cdrKey_t& key, int64_t* value) const;
    bool getInteger (const cdrKey_t& key, uint64_t* value) const;
    bool getInteger (const cdrKey_t& key, int32_t* value) const;
    bool getInteger (const cdrKey_t& key, uint32_t* value) const;
    bool getInteger (const cdrKey_t& key, uint8_t* value) const;
    bool getInteger (const cdrKey_t& key, int8_t* value) const;
    bool getInteger (const cdrKey_t& key, int16_t* value) const;
    bool getInteger (const cdrKey_t& key, uint16_t* value) const;

    /**
     * Assign the given cdrDateTime/time_t value to the key.
     * values
     *
     * @param key
     * @param value time_t/cdrDateTime to assign to key
     */
    void setDateTime (const cdrKey_t& key, const cdrDateTime& value);
    void setDateTime (const cdrKey_t& key, time_t value);

    /**
     * retrieve the value at the given key as a cdrDateTime/time_t/tm.
     * If that fails the function will return false.
     *
     * @param key
     * @param value cdrDateTime/time_t/tm value to assign to the key
     * @return whether the given key was present in the map, and the value
     * was/could be converted to a double.
     */
    bool getDateTime (const cdrKey_t& key, cdrDateTime& value) const;
    bool getDateTime (const cdrKey_t& key, time_t& value) const;
    bool getDateTime (const cdrKey_t& key, tm& value) const;

    /**
     * Create an empty array at the given key
     *
     * @param key
     */
    void emptyArray (const cdrKey_t& key);

    /**
     * Append given cdr into an array at the given key. If no array currently
     * exists then an empty array will be initialised at the key and the cdr
     * inserted
     *
     * @param key
     */
    void appendArray (const cdrKey_t& key, const cdr& data);

    /**
     * Get the size of the array at the given key. If the item at the given key
     * is not an array 0 is returned.
     *
     * @param key
     * @return size of array
     */
    u_int getArraySize (const cdrKey_t& key) const;

    /**
     * Assign the given cdr array to the key.
     *
     * @param key
     * @param value cdrArray to assign to key
     */
    void setArray (const cdrKey_t& key, cdrArray value);

    /**
     * Retrieve the cdrArry at key and assign to the given cdrArray value. If
     * the key is not found or the item is not a cdrArray false will be
     * returned, otherwise true.
     *
     * @param key
     * @param value cdrArray to assign value retrieved to
     * @return success
     */
    bool getArray (const cdrKey_t& key, const cdrArray** value) const;

    /**
     * Returns all items with assigned to given key. Iterates through all nested
     * cdrs looking for matching keys
     *
     * @param key
     * @return vector of cdrItems
     */
    vector<const cdrItem*> findAll (const cdrKey_t& key) const;

    /**
     * Return the size of the cdr were it to be serialized
     *
     */
    size_t serializedSize ()
    {
        char* data = NULL;
        size_t used = 0;
        serialize (data, used, false);
        return used;
    }

    /**
     * Serializes the given cdr to bytes for network send/file persistence. Set
     * write to false to determine the size of the serialized cdr only.
     *
     * @param cdr
     * @param data
     * @param used
     * @param write
     */
    bool serialize (char* data, size_t& used, bool write=true) const;

    /**
     * Deserializes the data buffer into a cdr object
     *
     * @param buffer
     * @param used
     */
    bool deserialize (const char* data, size_t& used);

    /**
     * Returns a string representation of the cdr
     *
     * @return string
     */
    string toString () const;

    class iterator
    {
    public:
        iterator (orderedItemMap::iterator it)
        : mIterator (it)
        {
        }

        ~iterator ()
        {
        }

        iterator (const iterator& other)
        : mIterator (other.mIterator)
        {
        }

        iterator& operator= (const iterator& other)
        {
            if (this != &other)
            {
                mIterator = other.mIterator;
            }
            return *this;
        }

        bool operator!= (const iterator& other) const
        {
            return mIterator != other.mIterator;
        }

        bool operator== (const iterator& other) const
        {
            return mIterator == other.mIterator;
        }

        pair<const cdrKey_t, cdrItem>* operator-> (void)
        {
            cdrItem* item = mIterator->second;
            if (item->mPair == NULL)
                item->mPair = new cdrItem::itemPair (item->mKey, *item);
            return item->mPair;
        }

        pair<const cdrKey_t, cdrItem> operator* (void)
        {
            cdrItem* item = mIterator->second;
            if (item->mPair == NULL)
                item->mPair = new cdrItem::itemPair (item->mKey, *item);
            return *item->mPair;
        }

        iterator& operator++ (void)
        {
            ++mIterator;
            return *this;
        }

        iterator operator++ (int)
        {
            iterator tmp (*this);
            ++mIterator;
            return tmp;
        }

    private:
        orderedItemMap::iterator mIterator;
    };

    class const_iterator
    {
    public:
        const_iterator (orderedItemMap::const_iterator it)
        : mIterator (it)
        {
        }

        const_iterator (const const_iterator& other)
        : mIterator (other.mIterator)
        {
        }

        ~const_iterator ()
        {
        }

        const_iterator& operator= (const const_iterator& other)
        {
            if (this != &other)
            {
                mIterator = other.mIterator;
            }
            return *this;
        }

        bool operator!= (const const_iterator& other) const
        {
            return mIterator != other.mIterator;
        }

        bool operator== (const const_iterator& other) const
        {
            return mIterator == other.mIterator;
        }

        const pair<const cdrKey_t, cdrItem>* operator-> (void)
        {
            cdrItem* item = mIterator->second;
            if (item->mPair == NULL)
                item->mPair = new cdrItem::itemPair (item->mKey, *item);
            return item->mPair;
        }

        const pair<const cdrKey_t, cdrItem> operator* (void)
        {
            cdrItem* item = mIterator->second;
            if (item->mPair == NULL)
                item->mPair = new cdrItem::itemPair (item->mKey, *item);
            return *item->mPair;
        }

        const_iterator& operator++ (void)
        {
            ++mIterator;
            return *this;
        }

        const_iterator operator++ (int)
        {
            const_iterator tmp (*this);
            ++mIterator;
            return tmp;
        }

    private:
        orderedItemMap::const_iterator mIterator;
    };

    const_iterator begin () const
    {
        return const_iterator (mOrdered.begin ());
    }

    const_iterator end () const
    {
        return const_iterator (mOrdered.end ());
    }

    iterator begin ()
    {
        return iterator (mOrdered.begin ());
    }

    iterator end ()
    {
        return iterator (mOrdered.end ());
    }

private:
    void findInChildren (const orderedItemMap* items,
                         const cdrKey_t& key,
                         vector<const cdrItem*>& found) const;

    bool serializeItem (const cdrItem* item,
                        char* data,
                        size_t& used,
                        uint16_t& fields,
                        bool write) const;

    bool deserializeItem (const char* data,
                          size_t& used);

    u_int          mNextIndex;
    orderedItemMap mOrdered;
    itemMap        mItems;
};

}

#endif /* _CDR_H_ */
