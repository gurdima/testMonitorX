#include "Settings/ResamplerSettings.hpp"
#include "Settings/SettingsParams.hpp"
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
namespace Settings
{

ResamplerSettings::ResamplerSettings()
: SettingsHolderBase(RESAMPLER_SECTION_NAME)
{
    _settings.add(new IntDatum(RESAMPLER_ENABLE, 0, true)); //{15.1}
    _settings.add(new IntDatum(RESAMPLER_OUTPUTBAND, 117000, true ));    //  
}

unsigned int ResamplerSettings::getStatusEnable() const
{
    return _settings.getInt(RESAMPLER_ENABLE)->getValue();
}

void ResamplerSettings::setStatusEnable(unsigned int val, unsigned int demMode)
{
    if(demMode == 0 && val == 3) {
        _settings.update(RESAMPLER_ENABLE, 1u);
    } else {
        _settings.update(RESAMPLER_ENABLE, 0u);
    }
}

unsigned int ResamplerSettings::getOutputBand() const
{
    return _settings.getInt(RESAMPLER_OUTPUTBAND)->getValue();
}

void ResamplerSettings::setOutputBand(unsigned int val)
{
    _settings.update(RESAMPLER_OUTPUTBAND, val);
}

} //namespace Settings
