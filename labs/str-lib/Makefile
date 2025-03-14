SHELL    = /bin/bash
CC       = gcc
CFLAGS   = -Wall
LDFLAGS  =
OBJFILES = strlib.o main.o
TARGET   = main

# Classify Requirements
GITHUB_USER       ?= demo
CLASSIFY_ENDPOINT ?= http://classify.obedmr.com/
EXECUTABLES        = curl jq

all: $(TARGET)

$(TARGET) : $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

submit: deps
	@if [[ $$(git ls-files --others --modified --exclude-standard | wc -l) -gt 0 ]]; then \
		echo "You have uncommitted files. Verify them and then, add and commit them."; \
		make clean; \
		git status; \
		exit -1; \
	fi
	@$(eval USER_ID := $(shell curl -s -k ${CLASSIFY_ENDPOINT}/users/\?githubID\=${GITHUB_USER} | jq -r '.users[0].ID'))
	@$(eval COMMIT_ID := $(shell git log --format='%H' -n 1))

	@curl -k -s -X POST -d "commit=$(COMMIT_ID)" $(CLASSIFY_ENDPOINT)/labs/$(USER_ID)/$$(basename "$$PWD") | jq

check-submission:
	@$(eval USER_ID := $(shell curl -s -k ${CLASSIFY_ENDPOINT}/users/\?githubID\=${GITHUB_USER} | jq -r '.users[0].ID'))
	@curl -k -s  $(CLASSIFY_ENDPOINT)/labs/$(USER_ID)/$$(basename "$$PWD") | jq

deps:
        $(foreach exec,$(EXECUTABLES),\
         $(if $(shell which $(exec)),,$(error "There's no '$(exec)' binary in your PATH")))

clean:
	rm -f $(OBJFILES) $(TARGET) *~
