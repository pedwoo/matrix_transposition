# .bashrc

# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi

# Uncomment the following line if you don't like systemctl's auto-paging feature:
# export SYSTEMD_PAGER=

module load gcc91
alias gcc=gcc-9.1.0
alias g++=g++-9.1.0

module load mpich-3.2.1--gcc-9.1.0

module load valgrind-3.15.0

# User specific aliases and functions
