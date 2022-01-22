# TP2_majeur_gettftp

Bonjour Monsieur,

Je n'ai pas reussi à executer correctement mon code car il me renvoie une erreur au niveau de sendTo() ligne 98.
Il me dit que l'adresse de destination est requise sauf que tous les arguments de la structure que je lui donne sont rempli plus haut par la fonction getaddrinfo qui s'execute sans erreures.

Le seul warning que j'ai lors de la compilation est une erreure de cast. Il attent un socklen_t *restrict et je lui donne juste quelque chose de type socklen_t.
Je ne parviens pas non plus à fixer cette erreure.
