#include <generic_css_hardware_model.hpp>

namespace Nos3
{
    REGISTER_HARDWARE_MODEL(Generic_cssHardwareModel,"GENERIC_CSS");

    extern ItcLogger::Logger *sim_logger;

    Generic_cssHardwareModel::Generic_cssHardwareModel(const boost::property_tree::ptree& config) : SimIHardwareModel(config), _enabled(0), _count(0), _config(0), _status(0)
    {
        /* Get the NOS engine connection string */
        std::string connection_string = config.get("common.nos-connection-string", "tcp://127.0.0.1:12001"); 
        sim_logger->info("Generic_cssHardwareModel::Generic_cssHardwareModel:  NOS Engine connection string: %s.", connection_string.c_str());

        /* Get a data provider */
        std::string dp_name = config.get("simulator.hardware-model.data-provider.type", "GENERIC_CSS_42_PROVIDER");
        _generic_css_dp = SimDataProviderFactory::Instance().Create(dp_name, config);
        sim_logger->info("Generic_cssHardwareModel::Generic_cssHardwareModel:  Data provider %s created.", dp_name.c_str());

        /* Get on a protocol bus */
        /* Note: Initialized defaults in case value not found in config file */
       
        std::string bus_name = "i2c_2";
        int i2c_bus_address = 64; // 0x40
        bus_name = config.get("simulator.hardware-model.i2c.bus", "i2c_2");
        i2c_bus_address = config.get("simulator.hardware-model.i2c.address", 64);

        _i2c_slave_connection = new I2CSlaveConnection(this, i2c_bus_address, connection_string, bus_name);
        sim_logger->info("Generic_cssHardwareModel::Generic_cssHardwareModel:  Now on i2c bus name %s, address %d.", bus_name.c_str(), i2c_bus_address);
    
        // Get on the command bus
        std::string time_bus_name = "command";
        if (config.get_child_optional("hardware-model.connections")) 
        {
            // Loop through the connections for the hardware model
            BOOST_FOREACH(const boost::property_tree::ptree::value_type &v, config.get_child("hardware-model.connections"))
            {
                // v.first is the name of the child
                // v.second is the child tree
                if (v.second.get("type", "").compare("time") == 0) // 
                {
                    time_bus_name = v.second.get("bus-name", "command");
                    // Found it... don't need to go through any more items
                    break; 
                }
            }
        }
        _time_bus.reset(new NosEngine::Client::Bus(_hub, connection_string, time_bus_name));
        sim_logger->info("Generic_cssHardwareModel::Generic_cssHardwareModel:  Now on time bus named %s.", time_bus_name.c_str());
        /* Construction complete */
        sim_logger->info("Generic_cssHardwareModel::Generic_cssHardwareModel:  Construction complete.");
    }


    Generic_cssHardwareModel::~Generic_cssHardwareModel(void)
    {        
        /* Close the protocol bus */

        /* Clean up the data provider */
        delete _generic_css_dp;
        _generic_css_dp = nullptr;

        /* The bus will clean up the time node */
    }

    void Generic_cssHardwareModel::run(void)
    {
        int i = 0;
        boost::shared_ptr<SimIDataPoint> dp;
        while(_keep_running)
        {
            sim_logger->info("Generic_cssHardwareModel::run:  Loop count %d, time %f", i++,
                _absolute_start_time + (double(_time_bus->get_time() * _sim_microseconds_per_tick)) / 1000000.0);
            dp = _generic_css_dp->get_data_point();
            sim_logger->info("Generic_cssHardwareModel::run:  %s", dp->to_string().c_str());

            sleep(5);
        }
    }

    /* Automagically set up by the base class to be called */
    void Generic_cssHardwareModel::command_callback(NosEngine::Common::Message msg)
    {
        // Get the data out of the message
        NosEngine::Common::DataBufferOverlay dbf(const_cast<NosEngine::Utility::Buffer&>(msg.buffer));
        sim_logger->info("Generic_cssHardwareModel::command_callback:  Received command: %s.", dbf.data);

        // Do something with the data
        std::string command = dbf.data;
        std::string response = "Generic_cssHardwareModel::command_callback:  INVALID COMMAND! (Try HELP)";
        boost::to_upper(command);
        if (command.compare("HELP") == 0) 
        {
            response = "Generic_cssHardwareModel::command_callback: Valid commands are HELP, ENABLE, DISABLE, STATUS=X, or STOP";
        }
        else if (command.compare("ENABLE") == 0) 
        {
            _enabled = GENERIC_CSS_SIM_SUCCESS;
            response = "Generic_cssHardwareModel::command_callback:  Enabled";
        }
        else if (command.compare("DISABLE") == 0) 
        {
            _enabled = GENERIC_CSS_SIM_ERROR;
            _count = 0;
            _config = 0;
            _status = 0;
            response = "Generic_cssHardwareModel::command_callback:  Disabled";
        }
        else if (command.substr(0,7).compare("STATUS=") == 0)
        {
            try
            {
                _status = std::stod(command.substr(7));
                response = "Generic_cssHardwareModel::command_callback:  Status set";
            }
            catch (...)
            {
                response = "Generic_cssHardwareModel::command_callback:  Status invalid";
            }            
        }
        else if (command.compare("STOP") == 0) 
        {
            _keep_running = false;
            response = "Generic_cssHardwareModel::command_callback:  Stopping";
        }

        // Send a reply
        sim_logger->info("Generic_cssHardwareModel::command_callback:  Sending reply: %s.", response.c_str());
        _command_node->send_reply_message_async(msg, response.size(), response.c_str());
    }

    I2CSlaveConnection::I2CSlaveConnection(Generic_cssHardwareModel* hm,
        int bus_address, std::string connection_string, std::string bus_name)
        : NosEngine::I2C::I2CSlave(bus_address, connection_string, bus_name)
    {
        _hardware_model = hm;
    }

    size_t I2CSlaveConnection::i2c_read(uint8_t *rbuf, size_t rlen)
    {
        size_t num_read;
        sim_logger->debug("i2c_read: 0x%02x", _i2c_out_data); // log data
        if(rlen <= 1)
        {
            rbuf[0] = _i2c_out_data;
            num_read = 1;
        }
        return num_read;
    }

    size_t I2CSlaveConnection::i2c_write(const uint8_t *wbuf, size_t wlen)
    {
        std::vector<uint8_t> in_data(wbuf, wbuf + wlen);
        sim_logger->debug("i2c_write: %s",
            SimIHardwareModel::uint8_vector_to_hex_string(in_data).c_str()); // log data
        //_i2c_out_data = _hardware_model->determine_i2c_response_for_request(in_data);
        return wlen;
    }
}
