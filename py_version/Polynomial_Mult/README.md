### Inverse-FFT Formula Annotation

$$
a_j = {\frac{1}{n}}\sum^{n-1}_{j=0}y_k{\omega}^{-kj}_{n}=B({\omega}^{-k}_{n})\\
B^{[0]}(x)=y_0+y_2x+...+y_{n-2}x^{\frac{n}{2}-1} \\
B^{[1]}(x)=y_1+y_3x+...+y_{n-1}x^{\frac{n}{2}-1} \\
$$

Thus, there is formula for `j < Len/2`:
$$
\begin{align}
a_j &= \frac{1}{n}(B^{[0]}({\omega}^{-2k}_{n}) + {\omega}^{-k}_{n}B^{[1]}({\omega}^{-2k}_{n})) \\
\\
&= \frac{1}{n}(B^{[0]}({\omega}^{-2k}_{n}) + {\omega}^{-k}_{n}B^{[1]}({\omega}^{-2k}_{n})) \\
\\
&= \frac{1}{2}*\frac{1}{\frac{1}{2}n}(B^{[0]}({\omega}^k_{\frac{n}{2}}) + {\omega}^{-k}_{n}B^{[1]}({\omega}^{-k}_{\frac{n}{2}})) \\
\\
&= \frac{1}{2}(B_{even}({\omega}^{\frac{-k}{2}}_{\frac{n}{2}}) + B_{odd}({\omega}^{\frac{-k}{2}}_{\frac{n}{2}}){\omega}^{-k}_n)
\\
\end{align}
$$
and for `j >= Len/2`:
$$
\begin{align}
a_j = \frac{1}{2}(B_{even}({\omega}^{\frac{-k}{2}}_{\frac{n}{2}}) - B_{odd}({\omega}^{\frac{-k}{2}}_{\frac{n}{2}}){\omega}^{-k}_n)
\end{align}
$$

