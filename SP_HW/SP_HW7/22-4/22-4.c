#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

void handler(int sig){
	printf("Signal %d received\n", sig);
	fflush(stdout);
}
typedef void(*sighandler_t)(int);
sighandler_t my_sigset(int sig, sighandler_t handler){
	sigset_t mask;
	struct sigaction sa,oldsa;
	// first get the current mask and see if sig is blocked,
	// the return value depends on this.
	int is_blocked = 0;
    if(sigprocmask(SIG_BLOCK, NULL, &mask)==-1){
        perror("my_sigset: sigprocmask1");
        return (sighandler_t)-1;
    }
    is_blocked = sigismember(&mask, sig);

	if(handler==NULL){	// should be ==SIG_HOLD but POSXI does not allow, so we use NULL to simualte
		// simply just add sig to the process' mask
		if(sigemptyset(&mask)==-1){
			perror("my_sigset: sigemptyset1");
			return (sighandler_t)-1;
		}
		if(sigaddset(&mask, sig)==-1){
			perror("my_sigset: sigaddset1");
			return (sighandler_t)-1;
		}
		if(sigprocmask(SIG_BLOCK, &mask, NULL)==-1){
			perror("my_sigset: sigprocmask2");
			return (sighandler_t)-1;
		}
		// get the old handler
		if(sigaction(sig,NULL,&oldsa)==-1){
			perror("my_sigset: sigaction1");
			return (sighandler_t)-1;
		}
	}
	else if(handler==SIG_IGN){	// ignores the signal
		sa.sa_handler = SIG_IGN;
		sa.sa_flags=0;
		if(sigemptyset(&sa.sa_mask)==-1){
			perror("my_sigset: sigemptyset2");
			return (sighandler_t)-1;
		}
		if(sigaction(sig, &sa, &oldsa)==-1){
			perror("my_sigset: sigaction2");
			return (sighandler_t)-1;
		}

		//Then remove the signal from the mask
		if(sigemptyset(&mask)==-1){
			perror("my_sigset: sigemptyset2");
			return (sighandler_t)-1;
		}
		if(sigaddset(&mask, sig)==-1){
			perror("my_sigset: sigaddset2");
			return (sighandler_t)-1;
		}
		if(sigprocmask(SIG_UNBLOCK, &mask, NULL)==-1){
			perror("my_sigset: sigprocmask3");
			return (sighandler_t)-1;
		}
	}
	else if(handler==SIG_DFL){	// reset the disposition of the signal to its default
		sa.sa_handler = SIG_DFL;
		sa.sa_flags=0;
		if(sigemptyset(&sa.sa_mask)==-1){
			perror("my_sigset: sigemptyset3");
			return (sighandler_t)-1;
		}
		if(sigaction(sig, &sa, &oldsa)==-1){
			perror("my_sigset: sigaction3");
			return (sighandler_t)-1;
		}
		//Then remove the signal from the mask
		if(sigemptyset(&mask)==-1){
			perror("my_sigset: sigemptyset3");
			return (sighandler_t)-1;
		}
		if(sigaddset(&mask, sig)==-1){
			perror("my_sigset: sigaddset3");
			return (sighandler_t)-1;
		}
		if(sigprocmask(SIG_UNBLOCK, &mask, NULL)==-1){
			perror("my_sigset: sigprocmask4");
			return (sighandler_t)-1;
		}
	}
	else{
		sa.sa_handler = handler;
		sa.sa_flags=0;
		if(sigemptyset(&sa.sa_mask)==-1){
			perror("my_sigset: sigemptyset4");
			return (sighandler_t)-1;
		}
		if(sigaction(sig, &sa, &oldsa)==-1){
			perror("my_sigset: sigaction4");
			return (sighandler_t)-1;
		}
		//Then remove the signal from the mask
		if(sigemptyset(&mask)==-1){
			perror("my_sigset: sigemptyset4");
			return (sighandler_t)-1;
		}
		if(sigaddset(&mask, sig)==-1){
			perror("my_sigset: sigaddset4");
			return (sighandler_t)-1;
		}
		if(sigprocmask(SIG_UNBLOCK, &mask, NULL)==-1){
			perror("my_sigset: sigprocmask5");
			return (sighandler_t)-1;
		}
	}
	if(is_blocked)	return oldsa.sa_handler; //should be SIG_HOLD but POSIX doesn't allow so we return old handler instead.
	else	return oldsa.sa_handler;
}
int my_sighold(int sig){
	// sighold adds sig to the calling process's signal mask:
	sigset_t mask;
	if(sigemptyset(&mask)==-1){
		perror("my_sighold: sigemptyset");
		return -1;
	}
	if(sigaddset(&mask, sig)==-1){
		perror("my_sighold: sigaddset");
		return -1;
	}
	if(sigprocmask(SIG_BLOCK, &mask, NULL)==-1){
		perror("sigprocmask");
		return -1;
	}
	return 0;
}
int my_sigrelse(int sig){
	// sighold removes sig to the calling process's signal mask:
	sigset_t mask;
	if(sigemptyset(&mask)==-1){
		perror("my_sighold: sigemptyset");
		return -1;
	}
	if(sigaddset(&mask, sig)==-1){
		perror("my_sighold: sigaddset");
		return -1;
	}
	if(sigprocmask(SIG_UNBLOCK, &mask, NULL)==-1){
		perror("my_sighold: sigprocmask");
		return -1;
	}
	return 0;
}
int my_sigignore(int sig){
	// sets the discomposition of sig to SIG_IGN
	struct sigaction sa;
	if(sigemptyset(&sa.sa_mask)==-1){
		perror("my_sigignore: sigemptyset");
		return -1;
	}
	sa.sa_flags = 0;
	sa.sa_handler=SIG_IGN;
	if(sigaction(sig, &sa, NULL)==-1){
		perror("my_sigignore: sigaction");
		return -1;
	}
	return 0;
}
int my_sigpause(int sig){
	sigset_t currentmask;
	if(sigprocmask(SIG_SETMASK, NULL, &currentmask)==-1){
		perror("my_sigpause: sigprocmask");
		return -1;
	}
	if(sigdelset(&currentmask, sig)==-1){
		perror("my_sigpause: sigdelset");
		return -1;		
	}
	return sigsuspend(&currentmask);
}
int main() {
    my_sigset(SIGINT, handler);
    my_sighold(SIGINT);

    printf("SIGINT blocked for 5 sec. Press Ctrl+C now and it'll be in the pending queue.\n");
    fflush(stdout);
    sleep(5);
    printf("Blocking period over. about to unblock SIGINT:\n");

    my_sigrelse(SIGINT);
    printf("SIGINT unblocked for at most 5 sec. Press Ctrl+C now and it'll be handled immediately.\n");	//note that sleep() will be interrupted
    fflush(stdout);
    sleep(5);
    printf("Unblocking period over. Now let's try out sigignore:\n");
    fflush(stdout);


    my_sigignore(SIGINT);
    printf("SIGINT ignored for 5 sec. Press Ctrl+C now and it will be ignored.\n");
    fflush(stdout);
    sleep(5);
    printf("Ignoring period over.\n");
    fflush(stdout);

    printf("Program completed.\n");
    fflush(stdout);

    return 0;
}