/*
 * Copyright 2014-2018 Neueda Ltd.
 */
#include "cdr.h"

namespace neueda
{

bool
cdrItem::asString (string& value) const
{
    char tmp[256];
    switch (mType)
    {
    case CDR_STRING:
        value.assign (mString);
        return true;
    case CDR_DOUBLE:
        snprintf (tmp, sizeof tmp, "%f", mDouble);
        value.assign (tmp);
        return true;
    case CDR_INTEGER:
        snprintf (tmp, sizeof tmp, "%lld", (long long)mInteger);
        value.assign (tmp);
        return true;
    case CDR_DATETIME:
        snprintf (tmp,
                  sizeof tmp,
                  "%04u-%02u-%02u %02u:%02u:%02u.%04u",
                  mDateTime.mYear,
                  mDateTime.mMonth,
                  mDateTime.mDay,
                  mDateTime.mHour,
                  mDateTime.mMinute,
                  mDateTime.mSecond,
                  mDateTime.mNanosecond);
        value.assign (tmp);
        return true;
    case CDR_ARRAY:
        snprintf (tmp, sizeof tmp, "<array of %zu>", mArray.size ());
        value.assign (tmp);
        return true;
    default:
        return false;
    }
}

cdr::cdr () :
    mNextIndex (0)
{
}

cdr::cdr (const cdr& obj) :
    mNextIndex (obj.mNextIndex),
    mItems (obj.mItems)
{
    for (itemMap::iterator itr = mItems.begin();
         itr != mItems.end();
         ++itr)
    {
        mOrdered[itr->second.mIndex] = &itr->second;
    }
}

cdr&
cdr::operator= (const cdr& obj)
{
    if (this != &obj)
    {
        mNextIndex = obj.mNextIndex;
        mItems = obj.mItems;

        mOrdered.clear ();
        for (itemMap::iterator itr = mItems.begin();
             itr != mItems.end();
             ++itr)
        {
            mOrdered[itr->second.mIndex] = &itr->second;
        }
    }

    return *this;
}

void
cdr::findInChildren (const orderedItemMap* items,
                     const cdrKey_t& key,
                     vector<const cdrItem*>& found) const
{
    orderedItemMap::const_iterator it0 = items->begin ();
    for (; it0 != items->end (); ++it0)
    {
        if (it0->second->mKey != key)
            continue;
        const cdrItem* item = it0->second;
        if (it0->second->mType == CDR_ARRAY)
        {
            vector<cdr>::const_iterator it = item->mArray.begin ();
            for (; it != it0->second->mArray.end (); ++it)
                findInChildren (&it->mOrdered, key, found);
        }
        else
            found.push_back (item);
    }
}

void
cdr::clear ()
{
    mItems.clear ();
    mOrdered.clear ();
}

size_t
cdr::size () const
{
    return mOrdered.size ();
}

bool
cdr::contains (const cdrKey_t& key) const
{
    return mItems.find (key) != mItems.end ();
}

bool
cdr::isType (const cdrKey_t& key, cdrItemType type, size_t size) const
{
    const cdrItem* item = getItem (key);
    if (item == NULL || item->mType != type)
        return false;
    if (type == CDR_STRING && item->mString.size () < size)
        return false;
    return true;
}

void
cdr::setItem (const cdrKey_t& key, cdrItem& item)
{
    item.mKey = key;
    item.mIndex = mNextIndex++;

    if (mItems.count (key) != 0)
        mOrdered.erase (mItems[key].mIndex);

    mItems[key] = item;
    mOrdered[item.mIndex] = &mItems[key];
}

const cdrItem*
cdr::getItem (const cdrKey_t& key) const
{
    map<int64_t, cdrItem>::const_iterator it = mItems.find (key);
    if (it == mItems.end ())
        return NULL;
    return &it->second;
}

void
cdr::deleteItem (const cdrKey_t& key)
{
    if (mItems.count (key) != 0)
    {
        mOrdered.erase (mItems[key].mIndex);
        mItems.erase (key);
    }
}

void
cdr::setString (const cdrKey_t& key, const string& value)
{
    cdrItem item (CDR_STRING);
    item.mString.assign (value);
    setItem (key, item);
}

void
cdr::setString (const cdrKey_t& key, const char* fmt, ...)
{
    char* tmp;

    va_list ap;
    va_start (ap, fmt);
    if (vasprintf (&tmp, fmt, ap) < 0)
        abort ();
    va_end (ap);

    cdrItem item (CDR_STRING);
    item.mString.assign (tmp);
    setItem (key, item);

    free (tmp);
}

bool
cdr::getString (const cdrKey_t& key, string& value) const
{
    const cdrItem* item = getItem (key);
    if (item == NULL)
        return false;
    return item->asString (value);
}

void
cdr::setDouble (const cdrKey_t& key, double value)
{
    cdrItem item (CDR_DOUBLE);
    item.mDouble = value;
    setItem (key, item);
}

bool
cdr::getDouble (const cdrKey_t& key, double& value) const
{
    const cdrItem* item = getItem (key);
    if (item == NULL)
        return false;
    if (item->mType == CDR_STRING)
    {
        char* endptr;
        errno = 0;
        double dd = strtod (item->mString.c_str (), &endptr);
        if (errno == ERANGE || *endptr != '\0')
            return false;
        value = dd;
        return true;
    }
    else if (item->mType == CDR_DOUBLE)
    {
        value = item->mDouble;
        return true;
    }
    return false;
}

void
cdr::setInteger (const cdrKey_t& key, int64_t value)
{
    cdrItem item (CDR_INTEGER);
    item.mInteger = value;
    setItem (key, item);
}

bool
cdr::getInteger (const cdrKey_t& key, int64_t& value) const
{
    const cdrItem* item = getItem (key);
    if (item == NULL)
        return false;
    if (item->mType == CDR_STRING)
    {
        char* endptr;
        errno = 0;
        int64_t ii = strtoll (item->mString.c_str (), &endptr, 0);
        if ((errno == ERANGE && (ii == LLONG_MIN || ii == LLONG_MAX)) ||
            *endptr != '\0')
            return false;
        value = ii;
        return true;
    }
    else if (item->mType == CDR_INTEGER)
    {
        value = item->mInteger;
        return true;
    }
    return false;
}

bool
cdr::getInteger (const cdrKey_t& key, uint64_t& value) const
{
    int64_t tmp;
    if (!getInteger (key, tmp))
        return false;
    value = tmp;
    return true;
}

bool
cdr::getInteger (const cdrKey_t& key, int32_t& value) const
{
    int64_t tmp;
    if (!getInteger (key, tmp))
        return false;
    if (tmp < INT32_MIN || tmp > INT32_MAX)
        return false;
    value = tmp;
    return true;
}

bool
cdr::getInteger (const cdrKey_t& key, uint32_t& value) const
{
    int64_t tmp;
    if (!getInteger (key, tmp))
        return false;
    if (tmp < 0 || tmp > UINT32_MAX)
        return false;
    value = tmp;
    return true;
}

bool
cdr::getInteger (const cdrKey_t& key, int16_t& value) const
{
    int64_t tmp;
    if (!getInteger (key, tmp))
        return false;
    if (tmp < INT16_MIN || tmp > INT16_MAX)
        return false;
    value = tmp;
    return true;
}

bool
cdr::getInteger (const cdrKey_t& key, uint8_t& value) const
{
    int64_t tmp;
    if (!getInteger (key, tmp))
        return false;
    if (tmp < 0 || tmp > UINT8_MAX)
        return false;
    value = tmp;
    return true;
}

bool
cdr::getInteger (const cdrKey_t& key, int8_t& value) const
{
    int64_t tmp;
    if (!getInteger (key, tmp))
        return false;
    if (tmp < INT8_MIN || tmp > INT8_MAX)
        return false;
    value = tmp;
    return true;
}


bool
cdr::getInteger (const cdrKey_t& key, uint16_t& value) const
{
    int64_t tmp;
    if (!getInteger (key, tmp))
        return false;
    if (tmp < 0 || tmp > UINT16_MAX)
        return false;
    value = tmp;
    return true;
}

bool
cdr::getInteger (const cdrKey_t& key, int64_t* value) const
{
    int64_t tmp;
    if (!getInteger (key, tmp))
        return false;
    *value = tmp;
    return true;
}

bool
cdr::getInteger (const cdrKey_t& key, uint64_t* value) const
{
    uint64_t tmp;
    if (!getInteger (key, tmp))
        return false;
    *value = tmp;
    return true;
}

bool
cdr::getInteger (const cdrKey_t& key, int32_t* value) const
{
    int32_t tmp;
    if (!getInteger (key, tmp))
        return false;
    *value = tmp;
    return true;
}

bool
cdr::getInteger (const cdrKey_t& key, uint32_t* value) const
{
    uint32_t tmp;
    if (!getInteger (key, tmp))
        return false;
    *value = tmp;
    return true;
}

bool
cdr::getInteger (const cdrKey_t& key, int16_t* value) const
{
    int16_t tmp;
    if (!getInteger (key, tmp))
        return false;
    *value = tmp;
    return true;
}

bool
cdr::getInteger (const cdrKey_t& key, uint8_t* value) const
{
    uint8_t tmp;
    if (!getInteger (key, tmp))
        return false;
    *value = tmp;
    return true;
}

bool
cdr::getInteger (const cdrKey_t& key, int8_t* value) const
{
    int8_t tmp;
    if (!getInteger (key, tmp))
        return false;
    *value = tmp;
    return true;
}

bool
cdr::getInteger (const cdrKey_t& key, uint16_t* value) const
{
    uint16_t tmp;
    if (!getInteger (key, tmp))
        return false;
    *value = tmp;
    return true;
}

bool
cdr::getDateTime (const cdrKey_t& key, cdrDateTime& value) const
{
    const cdrItem* item = getItem (key);
    if (item == NULL || item->mType != CDR_DATETIME)
        return false;
    value = item->mDateTime;
    return true;
}

bool
cdr::getDateTime (const cdrKey_t& key, time_t& value) const
{
    tm tm;
    if (!getDateTime (key, tm))
        return false;
    value = mktime (&tm);
    return true;
}

bool
cdr::getDateTime (const cdrKey_t& key, tm& value) const
{
    cdrDateTime tmp;
    if (!getDateTime (key, tmp))
        return false;
    memset (&value, 0, sizeof value);
    value.tm_hour = tmp.mHour;
    value.tm_min = tmp.mMinute;
    value.tm_sec = tmp.mSecond;
    value.tm_mday = tmp.mDay;
    if (tmp.mMonth > 0)
        value.tm_mon = tmp.mMonth - 1;
    value.tm_year = tmp.mYear - 1900;
    return true;
}

void
cdr::setDateTime (const cdrKey_t& key, const cdrDateTime& value)
{
    cdrItem item (CDR_DATETIME);
    item.mDateTime = value;
    setItem (key, item);
}

void
cdr::setDateTime (const cdrKey_t& key, time_t value)
{
    tm tm;
    gmtime_r (&value, &tm);
    cdrItem item (CDR_DATETIME);
    item.mDateTime.mHour = tm.tm_hour;
    item.mDateTime.mMinute = tm.tm_min;
    item.mDateTime.mSecond = tm.tm_sec;
    item.mDateTime.mDay = tm.tm_mday;
    item.mDateTime.mMonth = tm.tm_mon + 1;
    item.mDateTime.mYear = tm.tm_year + 1900;
    setItem (key, item);
}

void
cdr::emptyArray (const cdrKey_t& key)
{
    cdrItem item (CDR_ARRAY);
    setItem (key, item);
}

void
cdr::appendArray (const cdrKey_t& key, const cdr& data)
{
    if (mItems.count (key) == 0 || mItems[key].mType != CDR_ARRAY)
        emptyArray (key);
    cdrItem* item = &mItems[key];
    if (item->mType == CDR_STRING)
        item->mString.clear ();
    item->mType = CDR_ARRAY;
    item->mArray.push_back (data);
}

u_int
cdr::getArraySize (const cdrKey_t& key) const
{
    const cdrItem* item = getItem (key);
    if (item == NULL || item->mType != CDR_ARRAY)
        return 0;
    return item->mArray.size ();
}

bool
cdr::getArray (const cdrKey_t& key, const cdrArray** value) const
{
    const cdrItem* item = getItem (key);
    if (item == NULL || item->mType != CDR_ARRAY)
        return false;
    *value = &item->mArray;
    return true;
}

void
cdr::setArray (const cdrKey_t& key, cdrArray value)
{
    if (mItems.count (key) == 0 || mItems[key].mType != CDR_ARRAY)
        emptyArray (key);
    cdrItem* item = &mItems[key];
    if (item->mType == CDR_STRING)
        item->mString.clear ();
    item->mArray = value;
}

vector<const cdrItem*>
cdr::findAll (const cdrKey_t& key) const
{
    vector<const cdrItem*> found;
    findInChildren (&mOrdered, key, found);
    return found;
}

void
cdr::update (cdr& d)
{
    for (cdr::iterator it = d.begin (); it != d.end (); ++it)
    {
        cdrItem item = it->second;
        this->setItem (item.mKey, item);
    }
}

string
cdr::toString () const
{
    stringstream ss;
    int count = 0;
    for (cdr::const_iterator it = begin (); it != end (); ++it)
    {
        if (count != 0)
            ss << ",";

        string value;
        if (!it->second.asString (value))
            continue;

        if (it->second.mType == CDR_ARRAY)
        {
            for (cdrArray::const_iterator aIt = it->second.mArray.begin ();
                 aIt != it->second.mArray.end ();
                 ++aIt)
            {
                ss << it->first << "=[" << aIt->toString () << "]";
            }
        }
        else
            ss << it->first << "=" << value;

        count++;
    }
    return ss.str ();
}

bool
cdr::serialize (char* data,
                size_t& used,
                bool write) const
{
    uint16_t* mFields = NULL;
    if (write)
        mFields = (uint16_t*)(data + used);

    used += sizeof (uint16_t);

    uint16_t fields = 0;

    for (itemMap::const_iterator itr = mItems.begin();
         itr != mItems.end();
         ++itr)
    {
        if (!serializeItem (&itr->second, data, used, fields, write))
            return false;
    }

    if (write)
        *mFields = fields;

    return true;
}

bool
cdr::deserialize (const char* data,
                  size_t& used)
{
    uint16_t fields = *((uint16_t*)(data + used));
    used += sizeof (uint16_t);
        
    int count = 0;
    while (count < fields)
    {
        if (!deserializeItem (data, used))
            return false;
        count++;
    }
    return true;
}

bool
cdr::serializeItem (const cdrItem* item,
                    char* data,
                    size_t& used,
                    uint16_t& fields,
                    bool write) const
{
    if (write)
        *((cdrKey_t *)(data + used)) = item->mKey;
    used += sizeof (cdrKey_t);

    if (write)
        *((int8_t *)(data + used)) = item->mType;
    used += sizeof (int8_t);

    switch (item->mType)
    {
        case CDR_INTEGER:
            if (write)
                *((int16_t *)(data + used)) = sizeof (int64_t);
            used += sizeof (int16_t);

            if (write)
                *((int64_t *)(data + used)) = item->mInteger;
            used += sizeof (int64_t);
            break;
        case CDR_DOUBLE:
            if (write)
                *((int16_t *)(data + used)) = sizeof (double);
            used += sizeof (int16_t);

            if (write)
                *((double *)(data + used)) = item->mDouble;
            used += sizeof (double);
            break;
        case CDR_STRING:
            if (write)
                *((int16_t *)(data + used)) = item->mString.size ();
            used += sizeof (int16_t);

            if (write)
                memcpy ((void*)(data + used), (void*)(item->mString.c_str()), item->mString.size ());
            used += item->mString.size ();
            break;
        case CDR_ARRAY:
        {
            int16_t elems = item->mArray.size();
            if (write)
                *((int16_t *)(data + used)) = elems;
            used += sizeof (int16_t);

            for (cdrArray::const_iterator itr = item->mArray.begin(); 
                 itr != item->mArray.end();
                 ++itr)
            {
                itr->serialize (data, used, write);
            }
            break;
        }
        case CDR_DATETIME:
        {
            if (write)
                *((int16_t *)(data + used)) = 2 * sizeof (int64_t);
            used += sizeof (int16_t);

            time_t t;
            if (!getDateTime (item->mKey, t))
                return false;

            if (write)
            {
                // write seconds from epoch and then nanos
                *((int64_t *)(data + used)) = (int64_t)t;
                *((int64_t *)(data + used + sizeof (int64_t))) = (int64_t)item->mDateTime.mNanosecond;
            }

            used += 2 * sizeof (int64_t);
            break;
        }
        default:
            return false;
    }

    fields++;
    return true;
}

bool
cdr::deserializeItem (const char* data, size_t& used)
{
    cdrKey_t key = *(cdrKey_t*)(data + used);
    used += sizeof (cdrKey_t);

    int8_t type = *(int8_t *)(data + used);
    used += sizeof (type);

    int16_t size = *(int16_t *)(data + used);
    used += sizeof (int16_t);

    switch (type)
    {
        case CDR_INTEGER:
        {
            int64_t val = *(int64_t *)(data + used);
            used += sizeof (int64_t);

            setInteger (key, val);
            break;
        }
        case CDR_DOUBLE:
        {
            double val = *(double *)(data + used);
            used += sizeof (double);

            setDouble (key, val);
            break;
        }
        case CDR_STRING:
        {
            string val ((data + used), size);
            used += size;

            setString (key, val);
            break;
        }
        case CDR_ARRAY:
        {
            cdrArray cdrs;
            for (int i = 0; i < size; i++)
            {
                cdr* c = new cdr ();
                c->deserialize (data, used);
                cdrs.push_back (*c);
            }
            setArray (key, cdrs);
            break;
        }
        case CDR_DATETIME:
        {
            int64_t epoch = *(int64_t *)(data + used);
            used += sizeof (int64_t);
            int64_t nanos = *(int64_t *)(data + used);
            used += sizeof (int64_t);

            tm tm;
            gmtime_r (&epoch, &tm);

            cdrItem item (CDR_DATETIME);
            item.mDateTime.mHour = tm.tm_hour;
            item.mDateTime.mMinute = tm.tm_min;
            item.mDateTime.mSecond = tm.tm_sec;
            item.mDateTime.mDay = tm.tm_mday;
            item.mDateTime.mMonth = tm.tm_mon + 1;
            item.mDateTime.mYear = tm.tm_year + 1900;
            item.mDateTime.mNanosecond = nanos;
            setItem (key, item);

            break;
        }
        default:
            return false;
    }
    return true;
}

}
