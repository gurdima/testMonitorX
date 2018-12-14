#pragma once
/*---------------------------------------------------------------------------*/
#include <set>
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
class TunerObserver
{
public:
    virtual ~TunerObserver()
    {
    }

//    virtual void onTunerStateChanged(bool inserted) = 0;
};
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
class TunerObservable
{
public:
    void addObserver(TunerObserver& observer)
    {
        _observers.insert(&observer);
    }

    void removeObserver(TunerObserver& observer)
    {
        _observers.erase(&observer);
    }
    /*
    void notifyTunerStateChanged(bool inserted)
    {
        for(ObserversContainer::iterator it = _observers.begin(); it != _observers.end(); ++it) {
            (*it)->onTunerStateChanged(inserted);
        }
    }
    */
private:
    typedef std::set<TunerObserver*> ObserversContainer;

    ObserversContainer _observers;
};

