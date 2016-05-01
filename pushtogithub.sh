#git remote add origin https://mame-erl:54417096b2ef4717d5c510efae785d33080819d8@github.com/mame-erl/raspberry-pi-sensor-node.git
echo "Bitte commit message für GitHub eingeben:"
read message
git add -A
git commit -m "$message" 
git push -u origin master
