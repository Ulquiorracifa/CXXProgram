#include <mongo/mongo_client.h>
// #include "tinyjson.hpp"
namespace engine::mongodb
{
    mongo_client::mongo_client(){}

    mongo_client::mongo_client(string hostName, string port)
    {
        m_hostName = hostName;
        m_port = port;
        s_pool = nullptr;
        // mongocxx::instance instance{};
        // bsoncxx::string::view_or_value url_string = std::format()
        // mongocxx::uri uri{url_string};
        // s_pool = new mongocxx::pool(uri);
    }

    mongo_client::~mongo_client()
    {
        delete s_pool;
        s_pool = nullptr;
        delete m_dbInstance;
    }

    void mongo_client::setHostName(string hostName)
    {
        m_hostName = hostName;
    }

    void mongo_client::setPort(string port)
    {
        m_port = port;
    }

    bool mongo_client::connectToHost()
    {
        if ((m_hostName != "") && (m_port != ""))
        {
            if (m_dbInstance)
            {
                delete m_dbInstance;
                m_dbInstance = nullptr;
            }
            string uri_ = "mongodb://" + m_hostName + ":" + m_port + "/?minPoolSize=" + std::to_string(minPoolSize) + "&maxPoolSize=" + std::to_string(maxPoolSize);
            mongocxx::uri uri{uri_};
            s_pool = new mongocxx::pool(uri);
            m_dbInstance = new mongocxx::instance();
            auto client = s_pool->acquire();
            return (bool)client;
        }
        return false;
    }

    mongocxx::pool::entry mongo_client::client()
    {
        return s_pool->acquire();
    }

    mongocxx::stdx::optional<bsoncxx::document::value> mongo_client::find_One(string db, string coll, document &filter, const mongocxx::v_noabi::options::find &options)
    {
        mongocxx::stdx::optional<bsoncxx::document::value> ret;

        auto client = this->client();
        if (m_dbInstance && (bool)client)
        {
            return (*client)[db.c_str()][coll.c_str()].find_one(filter.view(), options);
        }

        return ret;
    }

    mongocxx::cursor *mongo_client::find(string db, string coll, document &filter, const mongocxx::v_noabi::options::find &options)
    {
        auto client = this->client();
        if (m_dbInstance && client)
        {
            auto c = (*client)[db.c_str()][coll.c_str()].find(filter.view(), options);
            return new (std::nothrow) mongocxx::cursor(std::move(c));
        }

        return nullptr;
    }

    mongocxx::stdx::optional<mongocxx::result::insert_one> mongo_client::insert_One(string db, string coll, document &doc, const mongocxx::v_noabi::options::insert &options)
    {
        mongocxx::stdx::optional<mongocxx::result::insert_one> ret;

        auto client = this->client();
        if (m_dbInstance && client)
        {
            ret = (*client)[db.c_str()][coll.c_str()].insert_one(doc.view(), options);
        }

        return ret;
    }

    mongocxx::stdx::optional<mongocxx::result::update> mongo_client::update_One(string db, string coll, document &filter, document &value, const mongocxx::v_noabi::options::update &options)
    {
        mongocxx::stdx::optional<mongocxx::result::update> ret;
        auto client = this->client();
        if (m_dbInstance && client)
        {
            ret = (*client)[db.c_str()][coll.c_str()].update_one(
                filter.view(),
                bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("$set", value)),
                options);
        }

        return ret;
    }

    mongocxx::stdx::optional<mongocxx::result::update> mongo_client::update_Many(string db, string coll, document &filter, document &value, const mongocxx::v_noabi::options::update &options)
    {
        mongocxx::stdx::optional<mongocxx::result::update> ret;
        auto client = this->client();
        if (m_dbInstance && client)
        {
            ret = (*client)[db.c_str()][coll.c_str()].update_many(
                filter.view(),
                bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("$set", value)),
                options);
        }

        return ret;
    }

    mongocxx::stdx::optional<mongocxx::result::delete_result> mongo_client::delete_One(string db, string coll, document &filter, const mongocxx::v_noabi::options::delete_options &options)
    {
        mongocxx::stdx::optional<mongocxx::result::delete_result> ret;

        auto client = this->client();
        if (m_dbInstance && client)
        {
            ret = (*client)[db.c_str()][coll.c_str()].delete_one(filter.view(), options);
        }

        return ret;
    }

    mongocxx::stdx::optional<mongocxx::result::delete_result> mongo_client::delete_Many(string db, string coll, document &filter, const mongocxx::v_noabi::options::delete_options &options)
    {
        mongocxx::stdx::optional<mongocxx::result::delete_result> ret;

        auto client = this->client();
        if (m_dbInstance && client)
        {
            ret = (*client)[db.c_str()][coll.c_str()].delete_many(filter.view(), options);
        }

        return ret;
    }

    std::int64_t mongo_client::countDocument(string db, string coll, document &filter, const mongocxx::options::count &option)
    {
        auto client = this->client();
        return (*client)[db.c_str()][coll.c_str()].count_documents(filter.view(), option);
    }

}