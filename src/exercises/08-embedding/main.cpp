#include "main.h"
using namespace std;

#include "main.h"
using namespace std;

// Rotation of black hole horizon.
double chi = 0.8;

// Dimensions of ellipsoid
double a = 1;
double b = 2;
double c = 1;

// Distance between sphere centers of peanut.
double d = 0.9;

Grid grid;

void find_solution(int N_theta, int N_phi) {
	Grid grid_(N_theta, N_phi);
	grid = grid_;

	// shared_ptr<RoundSphereMetric> metric = make_shared<RoundSphereMetric>(chi);
	// shared_ptr<HorizonMetric> metric = make_shared<HorizonMetric>(chi);
	shared_ptr<EllipsoidMetric> metric = make_shared<EllipsoidMetric>(a, b, c);
	// shared_ptr<PeanutMetric> metric = make_shared<PeanutMetric>(d);

	shared_ptr<Grid3DFunction> e_theta = make_shared<Grid3DFunction>(grid);
	shared_ptr<Grid3DFunction> e_phi = make_shared<Grid3DFunction>(grid);
	shared_ptr<Grid3DFunction> embedding = make_shared<Grid3DFunction>(grid);

	double identifier = a*100 + b*10 + c;
	// double identifier = d;
	// double identifier = chi;

	run_factorization(metric, e_theta, e_phi);
	run_relaxation(e_theta, e_phi, get_commutator_rms, identifier);
	// run_relaxation(e_theta, e_phi, get_commutator_rms);
	run_integration(e_theta, e_phi, embedding);

	char embedding_filename[50];
	sprintf(embedding_filename, "./assets/embedding_%.1f.csv", identifier);
	// sprintf(embedding_filename, "./assets/embedding.csv");
	ofstream embedding_output(embedding_filename);
	for (int i = 0; i < grid.N_theta; i++) {
		for (int j = 0; j < grid.N_phi; j++) {
			embedding_output << embedding->x_values[i][j] << "," << embedding->y_values[i][j] << "," << embedding->z_values[i][j] << endl;
		}
	}
	embedding_output.close();

	printf("\n\nDone!\n");
}

int main() {
	// Temporarily run for only one resolution.
	find_solution(15, 60);

	// Vary grid space.
	// for (int N = 10; N <= 100; N += 20) {
	// 	printf("\n(Nx, Ny) = (%d, %d)\n", N, N);

	// 	find_solution(N, N);
	// }

	// Vary chi.
	// for (chi = 0; chi <= 1; chi += 0.1) {
	// 	printf("\nchi = %f\n", chi);

	// 	find_solution(25, 100);
	// }

	// Vary distance between spheres.
	// for (d = 1.2; d <= 1.4; d += 0.1) {
	// 	printf("\nd = %.1f\n", d);

	// 	find_solution(50, 50);
	// }

	// Vary ellipsoid dimensions.
	// for (b = 1; b <= 9; b += 1) {
	// 	printf("\nb = %.1f\n", b);

	// 	find_solution(15, 60);
	// }
}
