package kr.ac.kookmin.udclasses;

public class Order {
	private DVD dvdsOrdered[] = new DVD[10];
	
	private int numOrdered = 0;
	/**
	* @return the numOrdered
	*/
	public int getNumOrdered() {
		return numOrdered;
	}
	/**
	* @param numOrdered the numOrdered to set
	*/
	public void setNumOrdered(int numOrdered) {
		this.numOrdered = numOrdered;
	}
	
	public void addDVD(DVD disc) {
		// store the number of discs currently in the order
		int qty = getNumOrdered();
		// check that the order is not already full
		if (qty < 10) {
			// add the disc to the order
			dvdsOrdered[qty] = disc;
			// increment the number of discs ordered
			setNumOrdered(qty + 1);
		}
	}
	
	public float totalCost() {
		// store the number of discs currently in the order
		int num = getNumOrdered();
		// store the running total of the discs in the order
		float total = 0;
		// loop through the discs in the order
		for (int i = 0; i < num; i++) {
			total = total + dvdsOrdered[i].getCost();
		}
		return total;
	}
	
	public void removeDVD(DVD disc) {
		// store the number of discs currently in the order
		int qty = getNumOrdered();
		// check that the order is not empty
		if (qty > 0) {
			// loop through the discs in the order
			for (int i = 0; i < qty; i++) {
				// if you have found the correct disc in
				// the order
				if (disc.equals(dvdsOrdered[i])) {
					// remove the disc from the order
					dvdsOrdered[i] = null;
					// decrement the number of discs
					// ordered
					setNumOrdered(qty - 1);
					// exit the loop;
					break;
				}
			}
		}
	}
}
