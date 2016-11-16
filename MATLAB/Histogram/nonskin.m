clearvars;
blues=zeros([1,32]);
greens=zeros([1,32]);
reds=zeros([1,32]);
fileID=fopen('NHistogram.txt');
str=fgets(fileID);
Nnumbers=sscanf(str, '%d');
nsprob=zeros();
nsum=0;
for b=1:32
    for g=1:32
        for r=1:32
            num=Nnumbers((b-1)*32*32+(g-1)*32+r-1+1);
            nsum=nsum+num;
            blues(b)=blues(b)+num;
            greens(g)=greens(g)+num;
            reds(r)=reds(r)+num;
        end 
    end
end

figure(1);
title('RGB');
grid on; hold on;
plot(1:32,reds,'Color',[1,0,0]);
plot(1:32,greens,'Color',[0,1,0]);
plot(1:32,blues,'Color',[0,0,1]);