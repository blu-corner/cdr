#include "cdr/cdr.h"

namespace CommonDataRepresentation
{

bool
cdrItem::asString (std::string& value) const
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
                  mDateTime.mMillisecond);
        value.assign (tmp);
        return true;
    case CDR_ARRAY:
        snprintf (tmp, sizeof tmp, "<array of %zu>", mArray.size ());
        value.assign (tmp);
        return true;
    }
    return false;
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
    mNextIndex = obj.mNextIndex;
    mItems = obj.mItems;

    mOrdered.clear ();
    for (itemMap::iterator itr = mItems.begin();
         itr != mItems.end();
         ++itr)
    {
        mOrdered[itr->second.mIndex] = &itr->second;
    }

    return *this;
}

void
cdr::findInChildren (const orderedItemMap* items,
                     const cdrKey_t& key,
                     std::vector<const cdrItem*>& found) const
{
    orderedItemMap::const_iterator it0 = items->begin ();
    for (; it0 != items->end (); ++it0)
    {
        if (it0->second->mKey != key)
            continue;
        const cdrItem* item = it0->second;
        if (it0->second->mType == CDR_ARRAY)
        {
            std::vector<cdr>::const_iterator it = item->mArray.begin ();
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
    std::map<int64_t, cdrItem>::const_iterator it = mItems.find (key);
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
cdr::setString (const cdrKey_t& key, const std::string& value)
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
cdr::getString (const cdrKey_t& key, std::string& value) const
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
    if (tmp < 0)
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
    if (tmp < 0 || tmp > INT8_MAX)
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
    return mktime (&tm);
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
    value.tm_year = tmp.mYear + 1900;
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

std::vector<const cdrItem*>
cdr::findAll (const cdrKey_t& key) const
{
    std::vector<const cdrItem*> found;
    findInChildren (&mOrdered, key, found);
    return found;
}

void
cdr::update (cdr& d)
{
    for (cdr::const_iterator it = d.begin (); it != d.end (); ++it)
    {
        cdrItem item = it->second;
        this->setItem (item.mKey, item);
    }
}

std::string
cdr::toString () const
{
    std::stringstream ss;
    int count = 0;
    for (cdr::const_iterator it = begin (); it != end (); ++it)
    {
        if (count != 0)
            ss << ",";

        std::string value;
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

}
