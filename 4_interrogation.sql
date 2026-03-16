-- L’agent recherche le patient par son nom
SELECT *
FROM patient
WHERE patient_nom LIKE 'Dup%';

-- L’agent vérifie les patients nés entre 1980 et 2000
SELECT *
FROM patient
WHERE patient_date_naissance BETWEEN '1980-01-01' AND '2000-12-31';

-- L’agent consulte les différentes villes où habitent les patients
SELECT DISTINCT patient_adresse
FROM patient;

-- L’agent recherche certains patients précis par leur identifiant
SELECT *
FROM patient
WHERE patient_id IN (1,2,3);

-- L’agent affiche la liste des patients triés par nom
SELECT patient_nom, patient_prenom
FROM patient
ORDER BY patient_nom;

-- L’agent consulte les rendez-vous du patient
SELECT p.patient_nom, r.rdv_date, r.rdv_heure
FROM patient p
JOIN rdv r ON p.patient_id = r.patient_id;

-- L’agent consulte le médecin associé au rendez-vous
SELECT r.rdv_date, m.medecin_nom
FROM rdv r
JOIN medecin m ON r.medecin_id = m.medecin_id;

-- L’agent consulte le service du médecin
SELECT m.medecin_nom, s.service_nom
FROM medecin m
JOIN service s ON m.service_id = s.service_id;

-- L’agent consulte tous les patients et leurs rendez-vous (même ceux sans rendez-vous)
SELECT p.patient_nom, r.rdv_date
FROM patient p
LEFT JOIN rdv r ON p.patient_id = r.patient_id;

-- Le personnel consulte les hospitalisations avec les chambres
SELECT h.hospitalisation_id, c.chambre_numero
FROM hospitalisation h
JOIN chambre c ON h.chambre_id = c.chambre_id;

-- Liste des actes réalisés avec le nom du patient et du médecin
SELECT p.patient_nom, m.medecin_nom, a.acte_libelle
FROM acte a
JOIN patient p ON a.patient_id = p.patient_id
JOIN medecin m ON a.medecin_id = m.medecin_id;

-- Nombre d’actes réalisés par chaque médecin
SELECT medecin_id, COUNT(*) AS nombre_actes
FROM acte
GROUP BY medecin_id;

-- Médecins ayant réalisé plus d’un acte
SELECT medecin_id, COUNT(*) AS nombre_actes
FROM acte
GROUP BY medecin_id
HAVING COUNT(*) > 1;

-- Factures associées aux patients
SELECT f.facture_id, p.patient_nom, f.facture_montant
FROM facture f
JOIN patient p ON f.patient_id = p.patient_id;

-- Paiements associés aux factures
SELECT f.facture_id, pa.paiement_montant, pa.paiement_mode
FROM facture f
JOIN paiement pa ON f.facture_id = pa.facture_id;

-- Montant total des factures
SELECT SUM(facture_montant)
FROM facture;

-- Patients ayant au moins une facture
SELECT *
FROM patient
WHERE patient_id IN (
    SELECT patient_id
    FROM facture
);

-- Patients sans rendez-vous
SELECT *
FROM patient
WHERE patient_id NOT IN (
    SELECT patient_id
    FROM rdv
);

-- Médecins ayant réalisé des actes
SELECT *
FROM medecin
WHERE EXISTS (
    SELECT *
    FROM acte
    WHERE acte.medecin_id = medecin.medecin_id
);

-- Factures dont le montant est supérieur à au moins une autre facture
SELECT *
FROM facture
WHERE facture_montant > ANY (
    SELECT facture_montant
    FROM facture
);

-- Actes dont le tarif est supérieur à tous les actes hospitaliers
SELECT *
FROM acte
WHERE acte_tarif > ALL (
    SELECT acte_tarif
    FROM acte
    WHERE hospitalisation_id IS NOT NULL
);

-- Nombre de rendez-vous par médecin, afficher seulement ceux qui ont plus de 1 rendez-vous
SELECT medecin_id, COUNT(*) AS nombre_rdv
FROM rdv
GROUP BY medecin_id
HAVING COUNT(*) > 1;

-- Montant total des factures par patient, afficher seulement ceux dont le total dépasse 100 €
SELECT patient_id, SUM(facture_montant) AS total_factures
FROM facture
GROUP BY patient_id
HAVING SUM(facture_montant) > 100;

