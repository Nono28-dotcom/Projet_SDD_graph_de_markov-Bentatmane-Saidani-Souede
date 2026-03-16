INSERT INTO service VALUES (1,'Cardiologie');
INSERT INTO service VALUES (2,'Urgences');
INSERT INTO service VALUES (3,'Pediatrie');
INSERT INTO service VALUES (4,'Neurologie');

INSERT INTO patient VALUES
(1,'Dupont','Jean','1980-05-10','Homme','Paris','0612345678','jean.dupont@mail.com','1234567890123');

INSERT INTO patient VALUES
(2,'Martin','Claire','1992-03-15','Femme','Lyon','0623456789','claire.martin@mail.com','2234567890123');

INSERT INTO patient VALUES
(3,'Bernard','Lucas','1975-11-20','Homme','Marseille','0634567890','lucas.bernard@mail.com','3234567890123');

INSERT INTO patient VALUES
(4,'Petit','Emma','2001-07-02','Femme','Nice','0645678901','emma.petit@mail.com','4234567890123');

INSERT INTO patient VALUES
(5,'Moreau','Paul','1988-09-18','Homme','Bordeaux','0656789012','paul.moreau@mail.com','5234567890123');


INSERT INTO chambre VALUES (1,101,1);
INSERT INTO chambre VALUES (2,102,1);
INSERT INTO chambre VALUES (3,201,2);
INSERT INTO chambre VALUES (4,202,2);
INSERT INTO chambre VALUES (5,301,3);


INSERT INTO medecin VALUES (1,'Durand','Cardiologue',1,NULL);
INSERT INTO medecin VALUES (2,'Roux','Urgentiste',2,NULL);
INSERT INTO medecin VALUES (3,'Faure','Pediatre',3,NULL);
INSERT INTO medecin VALUES (4,'Lambert','Neurologue',4,NULL);
INSERT INTO medecin VALUES (5,'Girard','Cardiologue',1,1);


INSERT INTO hospitalisation VALUES
(1,'2024-01-10','2024-01-15',1,1,1);

INSERT INTO hospitalisation VALUES
(2,'2024-02-05','2024-02-08',2,3,2);

INSERT INTO hospitalisation VALUES
(3,'2024-03-01','2024-03-05',3,5,4);


INSERT INTO acte VALUES
(1,'ECG',120,1,1,1);

INSERT INTO acte VALUES
(2,'Consultation urgence',80,2,2,2);

INSERT INTO acte VALUES
(3,'Examen pediatrique',70,4,3,3);

INSERT INTO acte VALUES
(4,'IRM cerebrale',300,3,4,NULL);



INSERT INTO rdv VALUES
(1,'2024-04-10','10:00:00','Controle cardiologique',1,1);

INSERT INTO rdv VALUES
(2,'2024-04-11','11:30:00','Consultation urgence',2,2);

INSERT INTO rdv VALUES
(3,'2024-04-15','09:00:00','Suivi pediatrique',3,4);

INSERT INTO rdv VALUES
(4,'2024-04-20','14:00:00','Consultation neurologique',4,3);



INSERT INTO facture VALUES
(1,'2024-01-15',120,1,1);

INSERT INTO facture VALUES
(2,'2024-02-08',80,2,2);

INSERT INTO facture VALUES
(3,'2024-03-05',70,3,4);

INSERT INTO facture VALUES
(4,'2024-03-10',300,4,3);



INSERT INTO paiement VALUES
(1,120,'Carte',1);

INSERT INTO paiement VALUES
(2,80,'Especes',2);

INSERT INTO paiement VALUES
(3,70,'Cheque',3);

INSERT INTO paiement VALUES
(4,300,'Virement',4);