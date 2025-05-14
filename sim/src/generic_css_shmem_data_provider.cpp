#include <generic_css_shmem_data_provider.hpp>

namespace Nos3
{
    REGISTER_DATA_PROVIDER(GenericCssShmemDataProvider,"GENERIC_CSS_SHMEM_PROVIDER");

    extern ItcLogger::Logger *sim_logger;

    GenericCssShmemDataProvider::GenericCssShmemDataProvider(const boost::property_tree::ptree& config) : SimIDataProvider(config)
    {
        sim_logger->trace("GenericCssShmemDataProvider::GenericCssShmemDataProvider:  Constructor executed");
        const std::string shm_name = config.get("simulator.hardware-model.data-provider.shared-memory-name", "Blackboard");
        const size_t shm_size = sizeof(BlackboardData);
        bip::shared_memory_object shm(bip::open_or_create, shm_name.c_str(), bip::read_write);
        shm.truncate(shm_size);
        bip::mapped_region shm_region(shm, bip::read_write);
        _shm_region = std::move(shm_region); // don't let this go out of scope/get destroyed
        _blackboard_data = static_cast<BlackboardData*>(_shm_region.get_address());    
    }

    boost::shared_ptr<SimIDataPoint> GenericCssShmemDataProvider::get_data_point(void) const
    {
        boost::shared_ptr<Generic_cssDataPoint> dp;
        {
            bip::scoped_lock<bip::interprocess_mutex> lock(_blackboard_data->mutex);
            dp = boost::shared_ptr<Generic_cssDataPoint>(
                new Generic_cssDataPoint(_blackboard_data->CSSValid, _blackboard_data->CSSIllum));
            // lock is released when scope ends
        }
        sim_logger->debug("GenericCssShmemDataProvider::get_data_point: valid=%d/%d/%d/%d/%d/%d, illum=%f/%f/%f/%f/%f/%f",
            _blackboard_data->CSSValid[0], _blackboard_data->CSSValid[1], _blackboard_data->CSSValid[2], 
            _blackboard_data->CSSValid[3], _blackboard_data->CSSValid[4], _blackboard_data->CSSValid[5], 
            _blackboard_data->CSSIllum[0], _blackboard_data->CSSIllum[1], _blackboard_data->CSSIllum[2], 
            _blackboard_data->CSSIllum[3], _blackboard_data->CSSIllum[4], _blackboard_data->CSSIllum[5]);
        return dp;
    }
}
