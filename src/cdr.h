/*
 * Copyright 2014-2018 Neueda Ltd.
 *
 * Generated 17/04/18
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
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>

#define PRINTFLIKE(a, b) __attribute__ ((format (printf, a, b)))

namespace neueda
{

class cdr;
typedef std::vector<cdr> cdrArray;
typedef int64_t cdrKey_t;

struct cdrDateTime
{
    cdrDateTime () : mHour (0),
                     mMinute (0),
                     mSecond (0),
                     mMillisecond (0),
                     mDay (0),
                     mMonth (0),
                     mYear (0)
    {}

    u_int mHour;
    u_int mMinute;
    u_int mSecond;
    u_int mMillisecond;

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

struct cdrItem
{
    cdrItem () : mPair (NULL),
                 mType (CDR_STRING),
                 mDouble (0),
                 mInteger (0),
                 mKey(0)
    {}
    cdrItem (cdrItemType type) : mPair (NULL),
                                 mType (type),
                                 mDouble (0),
                                 mInteger (0),
                                 mKey(0)
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

    bool asString (std::string& value) const;

    cdrKey_t     mKey;
    u_int        mIndex;

    typedef std::pair<const cdrKey_t, cdrItem> itemPair;
    itemPair*   mPair;

    cdrItemType mType;
    std::string mString;
    double      mDouble;
    int64_t     mInteger;
    cdrDateTime mDateTime;
    cdrArray    mArray;
};

class cdr
{
private:
    typedef std::map<cdrKey_t, cdrItem> itemMap;
    typedef std::map<u_int, cdrItem*> orderedItemMap;

public:
    cdr ();
    cdr (const cdr& obj);
    cdr& operator= (const cdr& obj);

    void clear ();
    size_t size () const;

    void update (cdr& d);

    bool contains (const cdrKey_t& key) const;

    bool isType (const cdrKey_t& key,
                 cdrItemType type,
                 size_t size = 0) const;

    void setItem (const cdrKey_t& key, cdrItem& item);
    const cdrItem* getItem (const cdrKey_t& key) const;
    void deleteItem (const cdrKey_t& key);

    void setString (const cdrKey_t& key, const std::string& value);
    void setString (const cdrKey_t& key,
                    const char* fmt,
                    ...) PRINTFLIKE(3, 4);
    bool getString (const cdrKey_t& key, std::string& value) const;

    void setDouble (const cdrKey_t& key, double value);
    bool getDouble (const cdrKey_t& key, double& value) const;

    void setInteger (const cdrKey_t& key, int64_t value);
    
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

    bool getDateTime (const cdrKey_t& key, cdrDateTime& value) const;
    bool getDateTime (const cdrKey_t& key, time_t& value) const;
    bool getDateTime (const cdrKey_t& key, tm& value) const;
    void setDateTime (const cdrKey_t& key, const cdrDateTime& value);
    void setDateTime (const cdrKey_t& key, time_t value);

    void emptyArray (const cdrKey_t& key);
    void appendArray (const cdrKey_t& key, const cdr& data);
    u_int getArraySize (const cdrKey_t& key) const;
    bool getArray (const cdrKey_t& key, const cdrArray** value) const;
    void setArray (const cdrKey_t& key, cdrArray value);

    std::vector<const cdrItem*> findAll (const cdrKey_t& key) const;

    std::string toString () const;

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

        const_iterator& operator= (const const_iterator& other)
        {
            mIterator = other.mIterator;
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

        const std::pair<const cdrKey_t, cdrItem>* operator-> (void)
        {
            cdrItem* item = mIterator->second;
            if (item->mPair == NULL)
                item->mPair = new cdrItem::itemPair (item->mKey, *item);
            return item->mPair;
        }

        const std::pair<const cdrKey_t, cdrItem> operator* (void)
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

private:
    void findInChildren (const orderedItemMap* items,
                         const cdrKey_t& key,
                         std::vector<const cdrItem*>& found) const;

    u_int          mNextIndex;
    orderedItemMap mOrdered;
    itemMap        mItems;
};

}

#endif /* _CDR_H_ */
