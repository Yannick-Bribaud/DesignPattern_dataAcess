#ifndef BASEDONNEE_H
#define BASEDONNEE_H


#include <QObject>
#include <QtSql/QtSql>
#include <QtSql/QSqlDatabase>
#include <QMutex>
#include <QString>

#define DEBUG_BASEDEDONNEES

#define BDD_HOSTNAME        "localhost"
#define BDD_USERNAME        "monUsername"
#define BDD_PASSWORD        "monPassword"

class BaseDeDonnees : public QObject
{
    Q_OBJECT
    public:
        static BaseDeDonnees* getInstance(QString type="QMYSQL");
        static void detruireInstance();

        bool estConnecte();
        bool connecter(QString nomBase, QString username=BDD_USERNAME, QString password=BDD_PASSWORD, QString serveur=BDD_HOSTNAME);
        bool estOuvert();
        bool ouvrir(QString fichierBase);

        /* uniquement pour : UPDATE, INSERT et DELETE */
        bool executer(QString requete);
        /* uniquement pour : SELECT */
        bool recuperer(QString requete, QString &donnees); // 1 -> 1
        bool recuperer(QString requete, QStringList &donnees); // 1 -> 1..*
        bool recuperer(QString requete, QVector<QString> &donnees); // 1..* -> 1
        bool recuperer(QString requete, QVector<QStringList> &donnees); // 1..* -> 1..*

    private:
        BaseDeDonnees(QString type);
        ~BaseDeDonnees();

        static BaseDeDonnees*  baseDeDonnees;
        static QString         typeBase;
        static int             nbAcces;

        QSqlDatabase           db;
        QMutex                 mutex;
};





#endif // BASEDONNEE_H
