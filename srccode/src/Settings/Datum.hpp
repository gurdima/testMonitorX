#pragma once
/*---------------------------------------------------------------------------*/
#include "Poco/AutoPtr.h"
#include "Poco/Util/AbstractConfiguration.h"
#include "Poco/Exception.h"
/*---------------------------------------------------------------------------*/
#include <string>
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
namespace Settings
{

/**
 * @brief Class represents abstract settings datum that has 
 *        name, can be loaded from file and saved to file
 */
class AbstractDatum
{
public:

    explicit AbstractDatum(const std::string& name)
    : _name(name)
    , _saveable(false)
    {
    }

    AbstractDatum(const std::string& name, bool saveable)
    : _name(name)
    , _saveable(saveable)
    {
    }

    virtual ~AbstractDatum()
    {
    }

    /**
     * @brief Get datum name
     * @return const std::string& 
     */
    const std::string& getName() const
    {
        return _name;
    }

    /**
     * @brief Is datum supposed to be saved. True if datum should be 
     *        saved, false otherwise
     * @return bool 
     */
    bool isSaveable() const
    {
          return _saveable;
    }

    /**
     * @brief Save datum
     * @param config Configuration to save datum in
     */
    void save(Poco::Util::AbstractConfiguration* config) const
    {
        if(isSaveable()) {
            saveImpl(config);
        }
    }

    /**
     * @brief Load datum
     * @param config Configuration to load datum from
     */
    void load(Poco::Util::AbstractConfiguration* config)
    {
        loadImpl(config);
    }

protected:

    virtual void saveImpl(Poco::Util::AbstractConfiguration* config) const = 0;
    virtual void loadImpl(Poco::Util::AbstractConfiguration* config) = 0;

private:
    std::string _name;
    bool _saveable;
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/**
 * @brief Class template that represents real settings datum, 
 *        can be parametrized with bool, int, float, double or
 *        string.
 */
template <typename T>
class Datum : public AbstractDatum
{
public:

    explicit Datum(const std::string& name)
    : AbstractDatum(name, false)
    , _dfltValue(0)
    {
    }


    Datum(const std::string& name, const T& defaultValue, bool saveable)
    : AbstractDatum(name, saveable)
    , _dfltValue(defaultValue)
    {
    }

    /**
     * @brief Get datum value
     * @return const T& 
     * @retval Datum value 
     */
    const T& getValue() const
    {
        return _value;
    }

    /**
     * @brief Set datum value 
     * @param value Value to set 
     */
    void setValue(const T& value)
    {
        _value = value;
    }

    /**
     * @brief Get default datum value
     * @return const T& 
     * @retval Default datum value 
     */
    const T& getDefaultValue() const
    {
        return _dfltValue;
    }

    /**
     * @brief Set default datum value 
     * @param value Value to set 
     */
    void setDefaultValue(const T& value)
    {
        _dfltValue = value;
    }

protected:
    virtual void saveImpl(Poco::Util::AbstractConfiguration *config) const
    {
        doSave(config);
    }

    virtual void loadImpl(Poco::Util::AbstractConfiguration *config)
    {
        doLoad(config);
    }

private:

    void doSave(Poco::Util::AbstractConfiguration *config) const
    {
        throw Poco::NotImplementedException("Type not supported", 0);
    }

    void doLoad(Poco::Util::AbstractConfiguration *config)
    {
        throw Poco::NotImplementedException("Type not supported", 0);
    }


    T _value;
    T _dfltValue;
};

/*---------------------------------------------------------------------------*/

template <>
inline
void Datum<unsigned int>::doSave(Poco::Util::AbstractConfiguration* config) const
{
    config->setInt(getName(), getValue());
}

template <>
inline
void Datum<int>::doSave(Poco::Util::AbstractConfiguration* config) const
{
    config->setInt(getName(), getValue());
}

template <>
inline
void Datum<float>::doSave(Poco::Util::AbstractConfiguration* config) const
{
    config->setDouble(getName(), getValue());
}

template <>
inline
void Datum<double>::doSave(Poco::Util::AbstractConfiguration* config) const
{
    config->setDouble(getName(), getValue());
}

template <>
inline
void Datum<bool>::doSave(Poco::Util::AbstractConfiguration* config) const
{
    config->setBool(getName(), getValue());
}

template <>
inline
void Datum<std::string>::doSave(Poco::Util::AbstractConfiguration* config) const
{
    config->setString(getName(), getValue());
}

/*---------------------------------------------------------------------------*/

template <>
inline
void Datum<unsigned int>::doLoad(Poco::Util::AbstractConfiguration* config)
{
    setValue(config->getInt(getName(), _dfltValue));
}

template <>
inline
void Datum<int>::doLoad(Poco::Util::AbstractConfiguration* config)
{
    setValue(config->getInt(getName(), _dfltValue));
}

template <>
inline
void Datum<float>::doLoad(Poco::Util::AbstractConfiguration* config)
{
    setValue(config->getDouble(getName(), _dfltValue));
}

template <>
inline
void Datum<double>::doLoad(Poco::Util::AbstractConfiguration* config)
{
    setValue(config->getDouble(getName(), _dfltValue));
}

template <>
inline
void Datum<bool>::doLoad(Poco::Util::AbstractConfiguration* config)
{
    setValue(config->getBool(getName(), _dfltValue));
}

template <>
inline
void Datum<std::string>::doLoad(Poco::Util::AbstractConfiguration* config)
{
    setValue(config->getString(getName(), _dfltValue));
}
/*---------------------------------------------------------------------------*/

typedef Datum<unsigned int> UIntDatum;
typedef Datum<int>          IntDatum;
typedef Datum<float>        FloatDatum;
typedef Datum<double>       DoubleDatum;
typedef Datum<bool>         BoolDatum;
typedef Datum<std::string>  StringDatum;
// typedef DatumHex<std::string>  HexDatum;

} // namespace Settings

