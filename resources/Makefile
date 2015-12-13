#BORDE INTE ÄNDRAS
DIR_LOCAL_SSH=~/.ssh/
KEY_NAME=id_rsa.pub
HOSTS=tussilago.it.uu.se fries.it.uu.se yxan.it.uu.se
FIRST_HOST=$(word 1, $(HOSTS))

#LÅT STÅ, ÄNDRA INTE
DIR_REMOTE_SSH=~/.ssh/

ZIP_NAME=fladdermusen.zip
DIR_REMOTE=fladdermusen/

FILE_KEY=$(DIR_LOCAL_SSH)$(KEY_NAME)
FILE_KEYS=$(DIR_REMOTE_SSH)authorized_keys

CYAN=\033[0;46m
RED=\033[0;41m
GREEN=\033[0;42m
NC=\033[0m # No Color

gen:
	@echo "$(RED)[INFO] Skriv inte in någon passphrase när du skapar nycklarna, allting ska vara default.$(NC)"
	ssh-keygen -t rsa

deploy_key: $(FILE_KEY)
		@echo "$(RED)[INFO] Vad är ditt användarnamn till UU? [ex: abcd1234]: $(NC)"; \
		read username; \
		echo "$(RED)[INFO] Svara yes när ssh frågar om du verkligen vill koppla upp, skriv in ditt A-lösenord.$(NC)"; \
		echo "Laddar upp nyckel till $(FIRST_HOST)"; \
		cat $(FILE_KEY) | ssh $$username@$(FIRST_HOST) "mkdir -p $(DIR_REMOTE_SSH) $(DIR_REMOTE) && cat >> $(FILE_KEYS)"
		@echo "$(GREEN) DONE!$(NC)"

zip:
	@rm -f $(ZIP_NAME); \
	cd ../; \
	make clean; \
	zip -r resources/$(ZIP_NAME) *

upload: zip
	@echo "$(RED)Vad är ditt användarnamn till UU? [ex: abcd1234]: $(NC)"; \
	read username; \
	scp $(ZIP_NAME) $$username@$(FIRST_HOST):$(DIR_REMOTE); \
	ssh $$username@$(FIRST_HOST) "cd $(DIR_REMOTE); ls | grep -v $(ZIP_NAME) | xargs rm -rf; unzip -o $(ZIP_NAME) -d .; rm *.zip;"

test:
	@echo "$(RED)Vad är ditt användarnamn till UU? [ex: abcd1234]: $(NC)"; \
	read username; \
	@echo "$(RED)Kom ihåg att skriva YES om ssh frågar om du verkligen vill koppla upp dig."; \
	for host in $(HOSTS); do \
		echo "$(CYAN)Testing GC at $$host...$(NC)"; \
		ssh $$username@$$host "cd $(DIR_REMOTE); make test > $$host.result.txt;"; \
		echo "$(CYAN)Downloading results from $$host...$(NC)"; \
		scp $$username@$$host:$(DIR_REMOTE)$$host.result.txt ./$$host.result.txt; \
	done; \
	echo "$(GREEN)Tests are done."
