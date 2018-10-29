/*
 * Copyright 2014-2018 Neueda Ltd.
 */
%module(directors="1", thread="1") cdr

%{
#include <cdr.h>

#include <stdexcept>
#include <sstream>
#include <string>
%}

%include "std_string.i"
%include "stdint.i"
%include "std_vector.i"

// macros
%define __attribute__(x)
%enddef

%rename(Cdr) neueda::cdr;

// ignore reference api
%ignore neueda::cdr::getInteger (const cdrKey_t& key, int64_t& value) const;
%ignore neueda::cdr::getInteger (const cdrKey_t& key, uint64_t& value) const;
%ignore neueda::cdr::getInteger (const cdrKey_t& key, int32_t& value) const;
%ignore neueda::cdr::getInteger (const cdrKey_t& key, uint32_t& value) const;
%ignore neueda::cdr::getInteger (const cdrKey_t& key, int16_t& value) const;
%ignore neueda::cdr::getInteger (const cdrKey_t& key, uint16_t& value) const;
%ignore neueda::cdr::getInteger (const cdrKey_t& key, int8_t& value) const;
%ignore neueda::cdr::getInteger (const cdrKey_t& key, uint8_t& value) const;
%ignore neueda::cdr::getInteger (const cdrKey_t& key, int64_t* value) const;
%ignore neueda::cdr::getInteger (const cdrKey_t& key, uint64_t* value) const;
%ignore neueda::cdr::getInteger (const cdrKey_t& key, int32_t* value) const;
%ignore neueda::cdr::getInteger (const cdrKey_t& key, uint32_t* value) const;
%ignore neueda::cdr::getInteger (const cdrKey_t& key, int16_t* value) const;
%ignore neueda::cdr::getInteger (const cdrKey_t& key, uint16_t* value) const;
%ignore neueda::cdr::getInteger (const cdrKey_t& key, int8_t* value) const;
%ignore neueda::cdr::getInteger (const cdrKey_t& key, uint8_t* value) const;

// cant understand nested class const_iterator
%ignore neueda::cdr::const_iterator;
%ignore neueda::cdr::iterator;
%ignore neueda::cdr::begin;
%ignore neueda::cdr::end;
%ignore neueda::cdr::getDateTime (const cdrKey_t& key, time_t& value) const;
%ignore neueda::cdr::getDateTime (const cdrKey_t& key, tm& value) const;
%ignore neueda::cdr::setDateTime (const cdrKey_t& key, time_t value);

%template(cdrArray) std::vector<neueda::cdr>;

// make cdr feel like a managed language
%extend neueda::cdr {

    int64_t getInteger (const cdrKey_t& key) {
        int64_t value = 0;
        bool ok = self->getInteger (key, value);
        if (!ok)
        {
            std::ostringstream oss;
            oss << "failed to retrieve value for key: "
                << key;
            throw std::runtime_error (oss.str ());
        }
        
        return value;
    }

    int64_t getInt64 (const cdrKey_t& key) {
        int64_t value = 0;
        bool ok = self->getInteger (key, value);
        if (!ok)
        {
            std::ostringstream oss;
            oss << "failed to retrieve value for key: "
                << key;
            throw std::runtime_error (oss.str ());
        }
        
        return value;
    }

    uint64_t getUInt64 (const cdrKey_t& key) {
        uint64_t value = 0;
        bool ok = self->getInteger (key, value);
        if (!ok)
        {
            std::ostringstream oss;
            oss << "failed to retrieve value for key: "
                << key;
            throw std::runtime_error (oss.str ());
        }
        
        return value;
    }

    int32_t getInt32 (const cdrKey_t& key) {
        int32_t value = 0;
        bool ok = self->getInteger (key, value);
        if (!ok)
        {
            std::ostringstream oss;
            oss << "failed to retrieve value for key: "
                << key;
            throw std::runtime_error (oss.str ());
        }
        
        return value;
    }
    
    uint32_t getUInt32 (const cdrKey_t& key) {
        uint32_t value = 0;
        bool ok = self->getInteger (key, value);
        if (!ok)
        {
            std::ostringstream oss;
            oss << "failed to retrieve value for key: "
                << key;
            throw std::runtime_error (oss.str ());
        }
        
        return value;
    }

    int16_t getInt16 (const cdrKey_t& key) {
        int16_t value = 0;
        bool ok = self->getInteger (key, value);
        if (!ok)
        {
            std::ostringstream oss;
            oss << "failed to retrieve value for key: "
                << key;
            throw std::runtime_error (oss.str ());
        }
        
        return value;
    }
    
    uint16_t getUInt16 (const cdrKey_t& key) {
        uint16_t value = 0;
        bool ok = self->getInteger (key, value);
        if (!ok)
        {
            std::ostringstream oss;
            oss << "failed to retrieve value for key: "
                << key;
            throw std::runtime_error (oss.str ());
        }
        
        return value;
    }

    int8_t getInt8 (const cdrKey_t& key) {
        int8_t value = 0;
        bool ok = self->getInteger (key, value);
        if (!ok)
        {
            std::ostringstream oss;
            oss << "failed to retrieve value for key: "
                << key;
            throw std::runtime_error (oss.str ());
        }
        
        return value;
    }
    
    uint8_t getUInt8 (const cdrKey_t& key) {
        uint8_t value = 0;
        bool ok = self->getInteger (key, value);
        if (!ok)
        {
            std::ostringstream oss;
            oss << "failed to retrieve value for key: "
                << key;
            throw std::runtime_error (oss.str ());
        }
        
        return value;
    }

    double getDouble (const cdrKey_t& key) {
        double value = 0;
        bool ok = self->getDouble (key, value);
        if (!ok)
        {
            std::ostringstream oss;
            oss << "failed to retrieve value for key: "
                << key;
            throw std::runtime_error (oss.str ());
        }
        
        return value;
    }

    std::string getString (const cdrKey_t& key) {
        std::string value;
        bool ok = self->getString (key, value);
        if (!ok)
        {
            std::ostringstream oss;
            oss << "failed to retrieve value for key: "
                << key;
            throw std::runtime_error (oss.str ());
        }
        
        return value;
    }

    neueda::cdrDateTime getDateTime (const cdrKey_t& key) {
        neueda::cdrDateTime value;
        bool ok = self->getDateTime (key, value);
        if (!ok)
        {
            std::ostringstream oss;
            oss << "failed to retrieve value for key: "
                << key;
            throw std::runtime_error (oss.str ());
        }
        
        return value;
    }

    neueda::cdrArray* getArray (const cdrKey_t& key) {
        neueda::cdrArray* value = NULL;
        bool ok = self->getArray (key, (const neueda::cdrArray**) &value);
        if (!ok)
        {
            std::ostringstream oss;
            oss << "failed to retrieve cdrArray for key: "
                << key;
            throw std::runtime_error (oss.str ());
        }
        
        return value;
    }
};

%include "cdr.h"
