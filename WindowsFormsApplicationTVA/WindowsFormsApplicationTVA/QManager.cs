using System;
using System.Collections.Generic;
using System.Collections.Concurrent;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MongoDB.Bson;
using MongoDB.Driver;

namespace WindowsFormsApplicationTVA
{
   
    class QManager
    {

 
        public string session_connectionString = "mongodb://localhost";
        public string session_databaseName = "tva_session";
        public MongoClient session_client;
        public object clientLock = new object();

        public MongoClient MongoSessionClient
        {
            get
            {
                lock (clientLock)
                {
                    if (session_client != null)
                        return session_client;
                    session_client = new MongoClient(session_connectionString);
                    return session_client;

                }
            }
        }

        public MongoCollection<T> GetCollection<T>()
        {
            var server = MongoSessionClient.GetServer();
            var database = server.GetDatabase(session_databaseName);
            return database.GetCollection<T>(typeof(T).Name);

        }

        }

      


        
       
    }

    


