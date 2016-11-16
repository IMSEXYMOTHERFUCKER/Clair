clearvars;
blues=zeros([1,32]);
greens=zeros([1,32]);
reds=zeros([1,32]);
fileID=fopen('skinU1.txt');
str=fgets(fileID);
numbers=sscanf(str, '%d');
numbers(32768)=0;
H=zeros([32768 2]);S=zeros([32768 2]);V=zeros([32768 2]);
Y=zeros([32768 2]);R=zeros([32768 2]);B=zeros([32768 2]);
nR=zeros([32768 2]);nG=zeros([32768 2]);nB=zeros([32768 2]);
sum=0;
for b=1:32
    for g=1:32
        for r=1:32
            num=numbers((b-1)*32*32+(g-1)*32+r-1+1);
            blues(b)=blues(b)+num;
            greens(g)=greens(g)+num;
            reds(r)=reds(r)+num;
            
            sum=sum+num;
            
            H((b-1)*32*32+(g-1)*32+r-1+1,1)=acos(0.5*(2*r-g-b)/sqrt((r-g)*(r-g)+(r-b)*(g-b)));
            H((b-1)*32*32+(g-1)*32+r-1+1,2)=numbers((b-1)*32*32+(g-1)*32+r-1+1);
            S((b-1)*32*32+(g-1)*32+r-1+1,1)=1-3*(min(r,min(g,b)))/(r+g+b);
            S((b-1)*32*32+(g-1)*32+r-1+1,2)=numbers((b-1)*32*32+(g-1)*32+r-1+1);
            V((b-1)*32*32+(g-1)*32+r-1+1,1)=(r+g+b)/3;
            V((b-1)*32*32+(g-1)*32+r-1+1,2)=numbers((b-1)*32*32+(g-1)*32+r-1+1);
            
            Y((b-1)*32*32+(g-1)*32+r-1+1,1)=0.299*r+0.587*g+0.144*b;
            Y((b-1)*32*32+(g-1)*32+r-1+1,2)=numbers((b-1)*32*32+(g-1)*32+r-1+1);
            R((b-1)*32*32+(g-1)*32+r-1+1,1)=r-0.299*r-0.587*g-0.144*b;
            R((b-1)*32*32+(g-1)*32+r-1+1,2)=numbers((b-1)*32*32+(g-1)*32+r-1+1);
            B((b-1)*32*32+(g-1)*32+r-1+1,1)=b-0.299*r-0.587*g-0.144*b;
            B((b-1)*32*32+(g-1)*32+r-1+1,2)=numbers((b-1)*32*32+(g-1)*32+r-1+1);
            
            nR((b-1)*32*32+(g-1)*32+r-1+1,1)=r/(r+g+b);
            nR((b-1)*32*32+(g-1)*32+r-1+1,2)=numbers((b-1)*32*32+(g-1)*32+r-1+1);
            nG((b-1)*32*32+(g-1)*32+r-1+1,1)=g/(r+g+b);
            nG((b-1)*32*32+(g-1)*32+r-1+1,2)=numbers((b-1)*32*32+(g-1)*32+r-1+1);
            nB((b-1)*32*32+(g-1)*32+r-1+1,1)=b/(r+g+b);
            nB((b-1)*32*32+(g-1)*32+r-1+1,2)=numbers((b-1)*32*32+(g-1)*32+r-1+1);
        end 
    end
end
sum=sum/40;
nblues=log10(blues+1);
ngreens=log10(greens+1);
nreds=log10(reds+1);

figure(1);
title('RGB');
grid on; hold on;
plot(1:32,reds,'Color',[1,0,0]);
plot(1:32,greens,'Color',[0,1,0]);
plot(1:32,blues,'Color',[0,0,1]);

figure(2);
subplot(3,1,1)
title('H');
hold on; grid on;
scatter(H(:,1),H(:,2)/sum);
subplot(3,1,2)
title('S');
grid on; hold on;
scatter(S(:,1),S(:,2)/sum);
subplot(3,1,3)
title('V');
grid on; hold on;
scatter(V(:,1),V(:,2)/sum);

figure(3);
subplot(3,1,1)
title('Y');
hold on; grid on;
scatter(Y(:,1),Y(:,2)/sum);
subplot(3,1,2);
title('Cr');
grid on; hold on;
scatter(R(:,1),R(:,2)/sum);
subplot(3,1,3)
title('Cb');
grid on; hold on;
scatter(B(:,1),B(:,2)/sum);

figure(4);
subplot(3,1,1)
title('nR');
hold on; grid on;
scatter(nR(:,1),nR(:,2)/sum);
subplot(3,1,2)
title('nG');
grid on; hold on;
scatter(nG(:,1),nG(:,2)/sum);
subplot(3,1,3)
title('nB');
grid on; hold on;
scatter(nB(:,1),nB(:,2)/sum);

% figure(5);
% hold on; grid on;
% scatter3(nB(:,1),nR(:,1),nR(:,2)/sum,15,nR(:,2)/sum,'filled');
% colorbar;
% xlabel('normalised Blue');
% ylabel('normalised Red');
% zlabel('Value');