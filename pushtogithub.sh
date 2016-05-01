echo "Bitte commit message für GitHub eingeben:"
read message
git add -A
git commit -m "$message" 
git push -u origin master
