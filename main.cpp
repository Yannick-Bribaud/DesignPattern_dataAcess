
#include <QApplication>
#include "Basedonnee.h"


// Exemples : MySQL et SQLite

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Exemple n°1 : MySQL
    qDebug() << QString::fromUtf8("Exemple n°1 : MySQL");
    // Au choix :
    BaseDeDonnees *bddMySQL = BaseDeDonnees::getInstance("QMYSQL");
    //BaseDeDonnees *bddMySQL = BaseDeDonnees::getInstance(); // par défaut -> QMYSQL

    // Au choix :
    //bddMySQL->connecter("maBase");
    //bddMySQL->connecter("maBase", "root", "password");
    bddMySQL->connecter("maBase", "root", "password", "localhost");

    if(bddMySQL->estConnecte())
    {
        bool retour;
        QString requete;
        QString periode;
        QStringList seuils;
        QVector<QStringList> listeAlarmes;
        QVector<QString> mesures;
        QStringList alarme;

        // Exemple : récupère un champ d'un seul enregistrement
        // le champ lu est récupéré dans un QString

        // récupère la périodicité de la tâche acquisition des mesures
        requete = "SELECT periode FROM parametres";
        qDebug() << QString::fromUtf8("requête : ") << requete;

        retour = bddMySQL->recuperer(requete, periode);
        if(retour != false)
        {
            qDebug() << QString::fromUtf8("période : ") << periode;
        }
        else
        {
            qDebug() << QString::fromUtf8("erreur !");
        }

        // Exemple : récupère au moins deux champs d'un seul enregistrement
        // les champs lus sont récupérés dans un QStringList

        // récupère les seuils pour la gestion des alarmes
        requete = "SELECT min, max FROM seuils";
        qDebug() << QString::fromUtf8("requête : ") << requete;

        retour = bddMySQL->recuperer(requete, seuils);
        if(retour != false)
        {
            qDebug() << QString::fromUtf8("seuil : entre %1 et %2").arg(seuils.at(0)).arg(seuils.at(1));
        }
        else
        {
            qDebug() << QString::fromUtf8("erreur !");
        }

        // Exemple : récupère au moins deux champs de plusieurs enregistrements
        // les enregistrements sont récupérés dans un QVector de QStringList

        // récupère les dépassement de seuil
        requete = "SELECT date, heure, temperature FROM mesures WHERE temperature >= '" + seuils.at(1) + "' OR temperature <= '" + seuils.at(0) + "' ORDER BY heure ASC";
        qDebug() << QString::fromUtf8("requête : ") << requete;

        retour = bddMySQL->recuperer(requete, listeAlarmes);
        if(retour != false)
        {
            for(int i=0; i < listeAlarmes.size(); i++)
            {
                alarme = listeAlarmes.at(i);
                qDebug() << QString::fromUtf8("%1 %2 : %3").arg(alarme.at(0)).arg(alarme.at(1)).arg(alarme.at(2));
            }
        }
        else
        {
            qDebug() << QString::fromUtf8("erreur !");
        }

        // Exemple : récupère un champ de plusieurs enregistrements
        // les enregistrements sont récupérés dans un QVector de QString

        // récupère les mesures
        requete = "SELECT temperature FROM mesures ORDER BY heure ASC";
        qDebug() << QString::fromUtf8("requête : ") << requete;

        retour = bddMySQL->recuperer(requete, mesures);
        if(retour != false)
        {
            for(int i=0; i < mesures.size(); i++)
            {
                qDebug() << QString::fromUtf8("Température : %1 °C").arg(mesures.at(i));
            }
        }
        else
        {
            qDebug() << QString::fromUtf8("erreur !");
        }

        // Exemple : INSERTION d'un enregistrement
        QString dateDebut = "2015-06-17";
        QString heureDebut = "14:03:00";
        float temperature = 36.5;

        requete = "INSERT INTO mesures (date, heure, temperature) VALUES ('" + dateDebut + "', '" + heureDebut + "', '" + QString::number(temperature) + "')";
        qDebug() << QString::fromUtf8("requête : ") << requete;
        retour = bddMySQL->executer(requete);
        if(retour == false)
        {
            qDebug() << QString::fromUtf8("erreur insertion !");
        }

        // Exemple : MODIFICATION d'un enregistrement
        int nouvellePeriode = periode.toInt() * 10;
        requete = "UPDATE parametres SET periode='" + QString::number(nouvellePeriode) + "'";
        qDebug() << QString::fromUtf8("requête : ") << requete;
        retour = bddMySQL->executer(requete);
        if(retour == false)
        {
            qDebug() << QString::fromUtf8("erreur modification !");
        }

        // Exemple : SUPPRESSION d'un enregistrement
        requete = "DELETE FROM mesures WHERE id='1'";
        qDebug() << QString::fromUtf8("requête : ") << requete;
        retour = bddMySQL->executer(requete);
        if(retour == false)
        {
            qDebug() << QString::fromUtf8("erreur suppression !");
        }
    }

    BaseDeDonnees::detruireInstance();

    // Exemple n°2 : SQLite
    qDebug() << QString::fromUtf8("Exemple n°2 : SQLite");
    BaseDeDonnees *bddSQLite = BaseDeDonnees::getInstance("QSQLITE");

    bddSQLite->ouvrir("bd-sqlite.sqlite");

    if(bddSQLite->estOuvert())
    {
        bool retour;
        QString requete;
        QString periode;

        // Exemple : récupère un champ d'un seul enregistrement
        // le champ lu est récupéré dans un QString

        // récupère la périodicité de la tâche acquisition des mesures
        requete = "SELECT periode FROM parametres";
        qDebug() << QString::fromUtf8("requête : ") << requete;

        retour = bddSQLite->recuperer(requete, periode);
        if(retour != false)
        {
            qDebug() << QString::fromUtf8("période : ") << periode;
        }
        else
        {
            qDebug() << QString::fromUtf8("erreur !");
        }

        // etc ...
    }

    BaseDeDonnees::detruireInstance();
    return a.exec();
}
