clearvars;
DIM=32;
blues=zeros([1,DIM]);
greens=zeros([1,DIM]);
reds=zeros([1,DIM]);
fileID=fopen('hand_hist32.txt');
str=fgets(fileID);
numbers=sscanf(str, '%d');
numbers(DIM*DIM*DIM)=0;
H=zeros([DIM*DIM*DIM 2]);S=zeros([DIM*DIM*DIM 2]);V=zeros([DIM*DIM*DIM 2]);
Y=zeros([DIM*DIM*DIM 2]);R=zeros([DIM*DIM*DIM 2]);B=zeros([DIM*DIM*DIM 2]);
nR=zeros([DIM*DIM*DIM 2]);nG=zeros([DIM*DIM*DIM 2]);nB=zeros([DIM*DIM*DIM 2]);
sum=0;
for b=1:DIM
    for g=1:DIM
        for r=1:DIM
            num=numbers((b-1)*DIM*DIM+(g-1)*DIM+r-1+1);
            blues(b)=blues(b)+num;
            greens(g)=greens(g)+num;
            reds(r)=reds(r)+num;
            
            sum=sum+num;
            
            H((b-1)*DIM*DIM+(g-1)*DIM+r-1+1,1)=acos(0.5*(2*r-g-b)/sqrt((r-g)*(r-g)+(r-b)*(g-b)));
            H((b-1)*DIM*DIM+(g-1)*DIM+r-1+1,2)=numbers((b-1)*DIM*DIM+(g-1)*DIM+r-1+1)*DIM;
            S((b-1)*DIM*DIM+(g-1)*DIM+r-1+1,1)=1-3*(min(r,min(g,b)))/(r+g+b);
            S((b-1)*DIM*DIM+(g-1)*DIM+r-1+1,2)=numbers((b-1)*DIM*DIM+(g-1)*DIM+r-1+1)*DIM;
            V((b-1)*DIM*DIM+(g-1)*DIM+r-1+1,1)=(r+g+b)/3;
            V((b-1)*DIM*DIM+(g-1)*DIM+r-1+1,2)=numbers((b-1)*DIM*DIM+(g-1)*DIM+r-1+1)*DIM;
            
            Y((b-1)*DIM*DIM+(g-1)*DIM+r-1+1,1)=0.299*r+0.587*g+0.144*b;
            Y((b-1)*DIM*DIM+(g-1)*DIM+r-1+1,2)=numbers((b-1)*DIM*DIM+(g-1)*DIM+r-1+1)*DIM;
            R((b-1)*DIM*DIM+(g-1)*DIM+r-1+1,1)=r-0.299*r-0.587*g-0.144*b;
            R((b-1)*DIM*DIM+(g-1)*DIM+r-1+1,2)=numbers((b-1)*DIM*DIM+(g-1)*DIM+r-1+1)*DIM;
            B((b-1)*DIM*DIM+(g-1)*DIM+r-1+1,1)=b-0.299*r-0.587*g-0.144*b;
            B((b-1)*DIM*DIM+(g-1)*DIM+r-1+1,2)=numbers((b-1)*DIM*DIM+(g-1)*DIM+r-1+1)*DIM;
            
            nR((b-1)*DIM*DIM+(g-1)*DIM+r-1+1,1)=r/(r+g+b);
            nR((b-1)*DIM*DIM+(g-1)*DIM+r-1+1,2)=numbers((b-1)*DIM*DIM+(g-1)*DIM+r-1+1)*DIM;
            nG((b-1)*DIM*DIM+(g-1)*DIM+r-1+1,1)=g/(r+g+b);
            nG((b-1)*DIM*DIM+(g-1)*DIM+r-1+1,2)=numbers((b-1)*DIM*DIM+(g-1)*DIM+r-1+1)*DIM;
            nB((b-1)*DIM*DIM+(g-1)*DIM+r-1+1,1)=b/(r+g+b);
            nB((b-1)*DIM*DIM+(g-1)*DIM+r-1+1,2)=numbers((b-1)*DIM*DIM+(g-1)*DIM+r-1+1)*DIM;
        end 
    end
end

figure(1);
title('RGB');
grid on; hold on;
plot(1:DIM,reds,'Color',[1,0,0]);
plot(1:DIM,greens,'Color',[0,1,0]);
plot(1:DIM,blues,'Color',[0,0,1]);

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