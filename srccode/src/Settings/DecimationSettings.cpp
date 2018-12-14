#include "Settings/DecimationSettings.hpp"
#include "Settings/SettingsParams.hpp"
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
namespace Settings
{
DecimationSettings::DecimationSettings()
: SettingsHolderBase(S_DECIMATION_SECTION_NAME)
{
    _settings.add(new IntDatum(S_DECIMATION_RATE, 1, true));
}

//----------------- mutable -----------------------------

unsigned int DecimationSettings::getDecimationRate() const
{

    return _settings.getInt(S_DECIMATION_RATE)->getValue();
}

void DecimationSettings::setDecimationRate(unsigned int val)
{
    if(val==0) val = 1;
    if(val>5)  val = 5;
    _settings.update(S_DECIMATION_RATE, val);
}
//----------------------------------------------

} // namespace Settings
