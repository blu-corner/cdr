#include "cdr.h"
#include <ctype.h>

namespace neueda
{

bool
cdr::toJson (string& j,
             string& err) const
{
    Json::Value root;

    if (!toJsonValue (root, err))
        return false;

    Json::FastWriter fw;
    j.assign (fw.write (root));
    return true;
}

bool
cdr::toJsonValue (Json::Value& r,
                     string& err) const
{
    for (itemMap::const_iterator itr = mItems.begin();
         itr != mItems.end();
         ++itr)
    {
        if (!itemToJsonValue (&itr->second, r, err))
            return false;
    }
    return true;
}


bool
cdr::itemToJsonValue (const cdrItem* item,
                  Json::Value& root,
                  string& err) const
{
    char key[21];
    sprintf (key, "%" PRId32, item->mKey);

    switch (item->mType)
    {
        case CDR_INTEGER:
            root[key] = (Json::Int64)item->mInteger;
            break;
        case CDR_DOUBLE:
            root[key] = item->mDouble;
            break;
        case CDR_STRING:
            root[key] = item->mString;
            break;
        case CDR_ARRAY:
        {
            for (cdrArray::const_iterator itr = item->mArray.begin(); 
                 itr != item->mArray.end();
                 ++itr)
            {
                Json::Value d;
                if (!itr->toJsonValue (d, err))
                    return false;

                root[key].append (d);
            }
            break;
        }
        case CDR_DATETIME:
        {
            string dt;
            if (!item->asString (dt))
                return false;
            root[key] = dt;
            break;
        }
        default:
            return false;
    }
    return true;
}

bool
cdr::fromJson (const string& j, string& err)
{
    Json::Reader parser;
    Json::Value  root;

    if (!parser.parse (j, root))
    {
        err.assign ("failed to parse json content");
        return false;
    }

    if (!fromJsonValue (root, err))
        return false;

    return true;
}


bool
cdr::fromJsonValue (Json::Value& root, string& err)
{
    size_t i = 0;
    cdrKey_t key = 0;

    for (Json::ValueIterator it = root.begin ();
         it != root.end ();
         ++it)
    {
        key = 0;
        i = 0;

        while (it.name ()[i] != '\0')
        {
            if (!isdigit (it.name ()[i]))
            {
                err.assign ("numeric keys only supported");
                return false;
            }
            key = (key * 10) + (it.name ()[i++] - '0');
        }

        if (it->isInt ())
            setInteger (key, it->asInt64());
        else if (it->isString ())
            setString (key, it->asString ());
        else if (it->isDouble ())
            setDouble (key, it->asDouble ());
        else if (it->isObject ())
            fromJsonValue(*it, err);
        else if (it->isArray ())
        {
            for (Json::Value::ArrayIndex ai = 0;
                 ai != it->size (); ai++)
            {
                // we only support jsonObjects within the array
                if (!(*it)[ai].isObject ())
                {
                    err.assign ("unsupported structure");
                    return false;
                }

                cdr* e = new cdr();
                if (!e->fromJsonValue (*it, err))
                    return false;

                appendArray (key, *e);
            }

        }
        else
        {
            err.assign ("unsupported data type");
            return false;
        }
    }

    return true;
}

}
