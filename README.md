# PIPEX

# Sources

[Pourquoi je n'écris plus d'articles sur les projets de 42 - codequoi](https://www.codequoi.com/pourquoi-je-necris-plus-darticles-sur-les-projets-de-42/)

Tutoriels

[pipex tutorial — 42 project](https://csnotes.medium.com/pipex-tutorial-42-project-4469f5dd5901)

Very relevant tutorial

[[C] Programmation systeme: execve(), fork() et pipe() - Accueil](https://n-pn.fr/t/2318-c--programmation-systeme-execve-fork-et-pipe)

Pour la fonction execve

[Benjv.fr](https://www.benjv.fr/appels-systeme)

A propos des appels systèmes

# Les testeurs

[GitHub - vfurmane/pipex-tester: Project Tester - Pipex](https://github.com/vfurmane/pipex-tester)

[GitHub - gabcollet/pipex: Pipex project school 42](https://github.com/gabcollet/pipex)

Easy to read project

# Exemples de projets

[GitHub - m3zh/pipex: pipex - 42 project](https://github.com/m3zh/pipex)

Projet passant les tests sauf 1

[GitHub - tolmvad/pipex: Ecole 42 project](https://github.com/tolmvad/pipex)

Projet passant tous les tests avec bonus

[GitHub - cclaude42/pipex](https://github.com/cclaude42/pipex)

# Gestion des processus

```c
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

// Définition d'une macro pour un statut de fin
// particulier pour le fils (si on ne l'indique pas à la compilation
// avec -D CHILD_EXIT_CODE=[nombre], par défaut elle sera à 42 :
#ifndef CHILD_EXIT_CODE
# define CHILD_EXIT_CODE 42
#endif

// Définition d'une macro pour le statut de fin
// du fils auquel on s'attend
#define EXPECTED_CODE 42

void	routine_fils(pid_t pid)
{
	printf("\e[36mFils : Coucou! Je suis le fils. PID recu de fork = %d\e[0m\n", pid);
	printf("\e[36mFils : Je sors avec le statut de fin %d.\e[0m\n", CHILD_EXIT_CODE);
	exit(CHILD_EXIT_CODE);
}

void	routine_pere(pid_t pid)
{
	int	status;

	printf("Pere : Je suis le pere. PID recu de fork = %d\n", pid);
	printf("Pere : J'attends mon fils qui a le PID [%d].\n", pid);
	waitpid(pid, &status, 0); // Attendre le fils ou   wait(&status);
	printf("Pere : Mon fils est sorti avec le statut %d\n", status);

	if (WIFEXITED(status)) // Si le fils est sorti normalement
	{
		printf("Pere : Le statut de fin de mon fils est %d\n", WEXITSTATUS(status));
		if (WEXITSTATUS(status) == EXPECTED_CODE)
			printf("Pere : C'est le statut que j'attendais !\n");
		else
			printf("Pere : Je ne m'attendais pas a ce statut-la...\n");
	}
}

int	main(void)
{
	pid_t	pid; // Stocke le retour de fork
    static int test = 0;

    test += 1;
    printf("\nTEST = %d\n", test);

	pid = fork();
	if (pid == -1)
    {
		return (EXIT_FAILURE);
    }
	else if (pid == 0)
    {
        printf("\n______________ routine fils ______________\n");
		routine_fils(pid);
    }
    if (pid > 0)
	{
        printf("\n______________ routine pere ______________\n");
    	routine_pere(pid);
    }
    return (EXIT_SUCCESS);
}
```

La commande fork permet de créer un autre processus identique qui va s’executer en même temps. Tous se passe comme si il y avait 2 main qui s’exécutent en parallèle. L’un avec le pid du processus père, l’autre avec le pid du processus fils. La valeur de pid est 0 quand il s’agit du processus fils et correspond à celle du processus fils quand c’est le père qui est actif.

# Les fonctions autorisées

## dup2()

La fonction int dup2(filde1, filde2) permet de changer les fichier de destination des différents file descriptor pour le processus en cours.

## unlink()

The unlink() function removes the link named by path from its directory and decrements the link count of the file which was referenced by the link.  If that decrement reduces the link count of the file to zero, and no process has the file open, then all resources associated with the file are reclaimed. If one or more process have the file open when the last link is removed, the link is removed, but the removal of the file is delayed until all references to it have been closed.

La fonction `unlink()` en C est utilisée pour supprimer un fichier existant. La fonction prend un argument, qui est le nom du fichier à supprimer.

Voici la signature de la fonction `unlink()` en C :

```c
int unlink(const char *pathname);
```

La fonction `unlink()` retourne 0 si la suppression du fichier est réussie, et -1 en cas d'erreur.

Voici quelques utilisations courantes de la fonction `unlink()` en C :

1. Supprimer un fichier existant :

```c
if (unlink("monfichier.txt") == -1) {
    perror("Erreur lors de la suppression du fichier");
}
```

1. Supprimer un fichier temporaire créé par le programme :

```c
char filename[] = "monfichierXXXXXX";
int fd = mkstemp(filename); // Créer un fichier temporaire
close(fd); // Fermer le fichier
unlink(filename); // Supprimer le fichier
```

Dans ce cas, `mkstemp()` est utilisé pour créer un fichier temporaire avec un nom unique. Une fois que le fichier est créé et fermé, `unlink()` est utilisé pour supprimer le fichier.

Il est important de noter que la suppression d'un fichier avec `unlink()` est permanente et irréversible. Il n'est pas possible de récupérer le fichier après l'avoir supprimé avec cette fonction.

## waitpid()

```c
pid_t waitpid(pid_t pid, int *status, int options);
```

- **status** : un pointeur vers une variable de type `int`, dans lequel `wait` et `waitpid` peuvent stoker le statut de fin du processus fils qu’on récupère. On
pourra ensuite analyser ce nombre pour déterminer si le fils a
correctement terminé son exécution ou s’il a été interrompu par exemple.
- **pid** : le PID du processus fils qu’on doit attendre. Le processus père connait ce PID grâce au retour de la fonction `fork` lors de la création du fils. On peut aussi spécifier -1 ici pour
attendre n’importe quel fils qui termine en premier, comme le fait `wait`. (Et en effet, `waitpid(-1, status, 0)` est exactement équivalent à `wait(status)`.)
- **options** : il existe quelques options possibles pour `waitpid`. Parmi elles, l’option parfois bien utile `WNOHANG`, qui force `waitpid` à retourner immédiatement si le fils n’a pas fini son exécution. Sans
cette option, le processus père restera par défaut bloqué tant que le
fils n’a pas terminé.

## access()

Bien sûr, voici un exemple simple d'utilisation de la fonction `access()` en C:

```c
#include <stdio.h>#include <stdlib.h>#include <unistd.h>int main() {
    // Vérifiez si le fichier "example.txt" peut être lu
    if (access("example.txt", R_OK) == 0) {
        printf("Le fichier peut être lu.\n");
    } else {
        printf("Le fichier ne peut pas être lu.\n");
    }

    // Vérifiez si le répertoire "examples" peut être écrit
    if (access("examples", W_OK) == 0) {
        printf("Le répertoire peut être écrit.\n");
    } else {
        printf("Le répertoire ne peut pas être écrit.\n");
    }

    return 0;
}
```

Dans cet exemple, nous utilisons la fonction `access()` pour vérifier si un fichier peut être lu et si un répertoire peut être écrit. Nous utilisons les constantes `R_OK` et `W_OK` pour spécifier les autorisations que nous voulons vérifier. Si `access()` renvoie 0, cela signifie que les autorisations sont accordées, sinon cela signifie qu'elles ne le sont pas.

## pipe()

`pipe()` en C permet de créer un canal de communication unidirectionnel entre deux processus. Elle est souvent utilisée pour rediriger la sortie standard (stdout) d'un processus vers l'entrée standard (stdin) d'un autre processus, afin que les deux processus puissent communiquer entre eux.

Le processus parent crée un tube en appelant la fonction `pipe()`. Cette fonction retourne deux descripteurs de fichiers : l'un pour l'extrémité de lecture et l'autre pour l'extrémité d'écriture du tube. Le processus parent peut alors créer un processus enfant à l'aide de la fonction `fork()`.

Le processus parent peut ensuite écrire des données dans l'extrémité d'écriture du tube à l'aide de la fonction `write()`, tandis que le processus enfant peut lire ces données à partir de l'extrémité de lecture du tube en utilisant la fonction `read()`. De cette manière, les deux processus peuvent communiquer en temps réel.

Voici un exemple simple d'utilisation de la fonction `pipe()` en C :

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd[2];
    pid_t pid;
    char buffer[20];

    if (pipe(fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        /* processus enfant */
        close(fd[1]); /* ferme l'extrémité d'écriture du tube */
        read(fd[0], buffer, sizeof(buffer));
        printf("Le processus enfant a reçu : %s\n", buffer);
        close(fd[0]); /* ferme l'extrémité de lecture du tube */
        exit(EXIT_SUCCESS);
    } else {
        /* processus parent */
        close(fd[0]); /* ferme l'extrémité de lecture du tube */
        write(fd[1], "Hello, world!", 14);
        close(fd[1]); /* ferme l'extrémité d'écriture du tube */
        wait(NULL); /* attend la fin du processus enfant */
        exit(EXIT_SUCCESS);
    }

    return 0;
}

```

Dans cet exemple, le processus parent écrit la chaîne de caractères "Hello, world!" dans l'extrémité d'écriture du tube, tandis que le processus enfant lit cette chaîne de caractères à partir de l'extrémité de lecture du tube et l'affiche à l'écran.

## strrerror()

Cette fonction renvoie la chaine de caractère correspondant au code erreur fournis en paramètre. La liste est la suivante.

[errors_index.txt](PIPEX%20111ff9046baa4065b653e4ddf85db3f0/errors_index.txt)

0 = "Undefined error: 0"
1 = "Operation not permitted"
2 = "No such file or directory"
3 = "No such process"
4 = "Interrupted system call"
5 = "Input/output error"
6 = "Device not configured"
7 = "Argument list too long"
8 = "Exec format error"
9 = "Bad file descriptor"
10 = "No child processes"
11 = "Resource deadlock avoided"
12 = "Cannot allocate memory"
13 = "Permission denied"
14 = "Bad address"
15 = "Block device required"
16 = "Resource busy"
17 = "File exists"
18 = "Cross-device link"
19 = "Operation not supported by device"
20 = "Not a directory"
21 = "Is a directory"
22 = "Invalid argument"
23 = "Too many open files in system"
24 = "Too many open files"
25 = "Inappropriate ioctl for device"
26 = "Text file busy"
27 = "File too large"
28 = "No space left on device"
29 = "Illegal seek"
30 = "Read-only file system"
31 = "Too many links"
32 = "Broken pipe"
33 = "Numerical argument out of domain"
34 = "Result too large"
35 = "Resource temporarily unavailable"
36 = "Operation now in progress"
37 = "Operation already in progress"
38 = "Socket operation on non-socket"
39 = "Destination address required"
40 = "Message too long"
41 = "Protocol wrong type for socket"
42 = "Protocol not available"
43 = "Protocol not supported"
44 = "Socket type not supported"
45 = "Operation not supported"
46 = "Protocol family not supported"
47 = "Address family not supported by protocol family"
48 = "Address already in use"
49 = "Can't assign requested address"
50 = "Network is down"
51 = "Network is unreachable"
52 = "Network dropped connection on reset"
53 = "Software caused connection abort"
54 = "Connection reset by peer"
55 = "No buffer space available"
56 = "Socket is already connected"
57 = "Socket is not connected"
58 = "Can't send after socket shutdown"
59 = "Too many references: can't splice"
60 = "Operation timed out"
61 = "Connection refused"
62 = "Too many levels of symbolic links"
63 = "File name too long"
64 = "Host is down"
65 = "No route to host"
66 = "Directory not empty"
67 = "Too many processes"
68 = "Too many users"
69 = "Disc quota exceeded"
70 = "Stale NFS file handle"
71 = "Too many levels of remote in path"
72 = "RPC struct is bad"
73 = "RPC version wrong"
74 = "RPC prog. not avail"
75 = "Program version wrong"
76 = "Bad procedure for program"
77 = "No locks available"
78 = "Function not implemented"
79 = "Inappropriate file type or format"
80 = "Authentication error"
81 = "Need authenticator"
82 = "Device power is off"
83 = "Device error"
84 = "Value too large to be stored in data type"
85 = "Bad executable (or shared library)"
86 = "Bad CPU type in executable"
87 = "Shared library version mismatch"
88 = "Malformed Mach-o file"
89 = "Operation canceled"
90 = "Identifier removed"
91 = "No message of desired type"
92 = "Illegal byte sequence"
93 = "Attribute not found"
94 = "Bad message"
95 = "EMULTIHOP (Reserved)"
96 = "No message available on STREAM"
97 = "ENOLINK (Reserved)"
98 = "No STREAM resources"
99 = "Not a STREAM"
100 = "Protocol error"
101 = "STREAM ioctl timeout"
102 = "Operation not supported on socket"
103 = "Policy not found"
104 = "State not recoverable"
105 = "Previous owner died"
106 = "Interface output queue is full"
107 = "Unknown error: 107"