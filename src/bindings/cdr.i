%module(directors="1", thread="1") Cdr

%{
#include <cdr/cdr.h>

#include <stdexcept>
#include <sstream>
#include <string>
%}

%include "std_string.i"
%include "stdint.i"
%include "std_vector.i"
%include "cdata.i"
%include "typemaps.i"
%include "exception.i"

// macros
%define __attribute__(x)
%enddef

%ignore CommonDataRepresentation::cdr::getInteger (const cdrKey_t& key, int64_t& value) const;
%ignore CommonDataRepresentation::cdr::getInteger (const cdrKey_t& key, int64_t* value) const;
%ignore CommonDataRepresentation::cdr::getInteger (const cdrKey_t& key, uint64_t& value) const;
%ignore CommonDataRepresentation::cdr::getInteger (const cdrKey_t& key, uint64_t* value) const;

%ignore CommonDataRepresentation::cdr::getInteger (const cdrKey_t& key, int32_t& value) const;
%ignore CommonDataRepresentation::cdr::getInteger (const cdrKey_t& key, int32_t* value) const;
%ignore CommonDataRepresentation::cdr::getInteger (const cdrKey_t& key, uint32_t& value) const;
%ignore CommonDataRepresentation::cdr::getInteger (const cdrKey_t& key, uint32_t* value) const;

%ignore CommonDataRepresentation::cdr::getInteger (const cdrKey_t& key, int16_t& value) const;
%ignore CommonDataRepresentation::cdr::getInteger (const cdrKey_t& key, int16_t* value) const;
%ignore CommonDataRepresentation::cdr::getInteger (const cdrKey_t& key, uint16_t& value) const;
%ignore CommonDataRepresentation::cdr::getInteger (const cdrKey_t& key, uint16_t* value) const;

%ignore CommonDataRepresentation::cdr::getInteger (const cdrKey_t& key, int8_t& value) const;
%ignore CommonDataRepresentation::cdr::getInteger (const cdrKey_t& key, int8_t* value) const;
%ignore CommonDataRepresentation::cdr::getInteger (const cdrKey_t& key, uint8_t& value) const;
%ignore CommonDataRepresentation::cdr::getInteger (const cdrKey_t& key, uint8_t* value) const;

// cant understand nested class const_iterator
%ignore CommonDataRepresentation::cdr::const_iterator;
%ignore CommonDataRepresentation::cdr::begin;
%ignore CommonDataRepresentation::cdr::end;
%ignore CommonDataRepresentation::cdr::getDateTime (const cdrKey_t& key, time_t& value) const;
%ignore CommonDataRepresentation::cdr::getDateTime (const cdrKey_t& key, tm& value) const;
%ignore CommonDataRepresentation::cdr::setDateTime (const cdrKey_t& key, time_t value);

%extend CommonDataRepresentation::cdr {

    int64_t getInt64 (const cdrKey_t& key) const {
        int64_t value = 0;
        bool ok = self->getInteger (key, value);
        if (not ok)
        {
            std::ostringstream oss;
            oss << "failed to find key: "
                << key;
            throw std::runtime_error (oss.str ());
        }
        
        return value;
    }

    uint64_t getUInt64 (const cdrKey_t& key) const {
        uint64_t value = 0;
        bool ok = self->getInteger (key, value);
        if (not ok)
        {
            std::ostringstream oss;
            oss << "failed to find key: "
                << key;
            throw std::runtime_error (oss.str ());
        }
        
        return value;
    }

    int32_t getInt32 (const cdrKey_t& key) const {
        int32_t value = 0;
        bool ok = self->getInteger (key, value);
        if (not ok)
        {
            std::ostringstream oss;
            oss << "failed to find key: "
                << key;
            throw std::runtime_error (oss.str ());
        }
        
        return value;
    }
    
    uint32_t getUInt32 (const cdrKey_t& key) const {
        uint32_t value = 0;
        bool ok = self->getInteger (key, value);
        if (not ok)
        {
            std::ostringstream oss;
            oss << "failed to find key: "
                << key;
            throw std::runtime_error (oss.str ());
        }
        
        return value;
    }

    int16_t getInt16 (const cdrKey_t& key) const {
        int16_t value = 0;
        bool ok = self->getInteger (key, value);
        if (not ok)
        {
            std::ostringstream oss;
            oss << "failed to find key: "
                << key;
            throw std::runtime_error (oss.str ());
        }
        
        return value;
    }
    
    uint16_t getUInt16 (const cdrKey_t& key) const {
        uint16_t value = 0;
        bool ok = self->getInteger (key, value);
        if (not ok)
        {
            std::ostringstream oss;
            oss << "failed to find key: "
                << key;
            throw std::runtime_error (oss.str ());
        }
        
        return value;
    }

    int8_t getInt8 (const cdrKey_t& key) const {
        int8_t value = 0;
        bool ok = self->getInteger (key, value);
        if (not ok)
        {
            std::ostringstream oss;
            oss << "failed to find key: "
                << key;
            throw std::runtime_error (oss.str ());
        }
        
        return value;
    }
    
    uint8_t getUInt8 (const cdrKey_t& key) const {
        uint8_t value = 0;
        bool ok = self->getInteger (key, value);
        if (not ok)
        {
            std::ostringstream oss;
            oss << "failed to find key: "
                << key;
            throw std::runtime_error (oss.str ());
        }
        
        return value;
    }

    double getDouble (const cdrKey_t& key) const {
        double value = 0;
        bool ok = self->getDouble (key, value);
        if (not ok)
        {
            std::ostringstream oss;
            oss << "failed to find key: "
                << key;
            throw std::runtime_error (oss.str ());
        }
        
        return value;
    }

    std::string getString (const cdrKey_t& key) const {
        std::string value;
        bool ok = self->getString (key, value);
        if (not ok)
        {
            std::ostringstream oss;
            oss << "failed to find key: "
                << key;
            throw std::runtime_error (oss.str ());
        }
        
        return value;
    }

    CommonDataRepresentation::cdrDateTime getDateTime (const cdrKey_t& key) const {
        CommonDataRepresentation::cdrDateTime value;
        bool ok = self->getDateTime (key, value);
        if (not ok)
        {
            std::ostringstream oss;
            oss << "failed to find key: "
                << key;
            throw std::runtime_error (oss.str ());
        }
        
        return value;
    }
    
};

%include "cdr/cdr.h"
