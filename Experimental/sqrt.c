int my_sqrt(int x)
{
        if (x <= 0) {
                return 0;
        }

        // Newton-Rapson method
        double xi = 1.0; // initial guess

        // iterate 64 times
        for (int i = 0; i < 64; i++) {
                xi = 0.5 * (xi + (x / xi));
        }

        return (int) xi;
}